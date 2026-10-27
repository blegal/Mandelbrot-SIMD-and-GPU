/*
 *  Copyright (c) 2026-... Bertrand LE GAL
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held
 *  liable for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute
 *  it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment
 *  in the product documentation would be appreciated but
 *  is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any
 *  source distribution.
 *
 */
#if defined(_ENABLE_METAL_)
/*
 *
 *
 *##############################################################################
 *
 *
 */
#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include "Metal.hpp"
#include "MetalEngine.hpp"

#include <cstring>
#include <iostream>
/*
 *
 *
 *##############################################################################
 *
 *
 */
//
// Source du kernel, genere par CMake a partir de mandelbrot.metal
//
#include "mandelbrot_metal_source.hpp"
/*
 *
 *
 *##############################################################################
 *
 *
 */
MTL::Device*       MetalEngine::_mDevice       = nullptr;
MTL::CommandQueue* MetalEngine::_mCommandQueue = nullptr;
/*
 *
 *
 *##############################################################################
 *
 *
 */
MetalEngine::MetalEngine(const std::string _name_, const std::string _kernel_, const bool _fastMath_) : Convergence(_name_)
{
    fractal      = "mandelbrot";
    modeSIMD     = "GPU";
    modeOPENMP   = "disable";
    OTHER        = "metal";

    kernel       = _kernel_;
    fastMath     = _fastMath_;

    _mPSO        = nullptr;
    _mBuffer     = nullptr;
    _mBufferSize = 0;
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
MetalEngine::~MetalEngine( )
{
    if( _mBuffer != nullptr ) _mBuffer->release();
    if( _mPSO    != nullptr ) _mPSO   ->release();
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
bool MetalEngine::init()
{
    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();

    //
    // On recherche la device par defaut dans le systeme (partagee entre moteurs)
    //

    if( _mDevice == nullptr )
    {
        _mDevice = MTL::CreateSystemDefaultDevice();
        if( _mDevice == nullptr )
        {
            pool->release();
            return false;
        }
        _mCommandQueue = _mDevice->newCommandQueue();
    }

    //
    // Compilation a la volee du code metal embarque dans l'executable
    //

    NS::Error*         error   = nullptr;
    MTL::CompileOptions* options = MTL::CompileOptions::alloc()->init();
    options->setFastMathEnabled( fastMath );

    NS::String*   source  = NS::String::string(MANDELBROT_METAL_SOURCE, NS::UTF8StringEncoding);
    MTL::Library* library = _mDevice->newLibrary(source, options, &error);
    options->release();
    if (library == nullptr)
    {
        std::cout << "(EE) Failed to compile the metal kernels : " << (error ? error->localizedDescription()->utf8String() : "?") << std::endl;
        pool->release();
        return false;
    }

    //
    // On part à la recherche de la fonction (metal) dans la bibliotheque
    //

    MTL::Function* function = library->newFunction( NS::String::string(kernel.c_str(), NS::UTF8StringEncoding) );
    if (function == nullptr)
    {
        std::cout << "(EE) Failed to find the [" << kernel << "] function in the library." << std::endl;
        library->release();
        pool->release();
        return false;
    }

    //
    // On cree les structures qui vont nous permettre de lancer les operations sur le GPU
    //

    _mPSO = _mDevice->newComputePipelineState(function, &error);
    function->release();
    library ->release();
    pool    ->release();

    if (_mPSO == nullptr)
    {
        std::cout << "(EE) Failed to created pipeline state object for [" << kernel << "]." << std::endl;
        return false;
    }
    return true;
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
void MetalEngine::run(const void* params, const size_t length, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
    const size_t IMAGE_SIZE = (size_t)IMAGE_WIDTH * IMAGE_HEIGHT;

    if( (_mPSO == nullptr) && (init() == false) )
    {
        std::cout << "(EE) The metal engine [" << name() << "] could not be initialized." << std::endl;
        exit( EXIT_FAILURE );
    }

    //
    //  Allocation (ou re-allocation si la taille de l'image change) de la zone
    //  mémoire permettant d'échanger avec le GPU (memoire unifiee CPU/GPU)
    //

    if( _mBufferSize != IMAGE_SIZE )
    {
        if( _mBuffer != nullptr ) _mBuffer->release();
        _mBuffer     = _mDevice->newBuffer(sizeof(float) * IMAGE_SIZE, MTL::ResourceStorageModeShared);
        _mBufferSize = IMAGE_SIZE;
    }

    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();

    //
    // Create a command buffer to hold commands and start a compute pass.
    //

    MTL::CommandBuffer*         commandBuffer  = _mCommandQueue->commandBuffer();
    MTL::ComputeCommandEncoder* computeEncoder = commandBuffer->computeCommandEncoder();

    computeEncoder->setComputePipelineState(_mPSO);
    computeEncoder->setBuffer(_mBuffer, 0, 0);
    computeEncoder->setBytes (params, length, 1);

    const NS::UInteger w = _mPSO->threadExecutionWidth();
    const NS::UInteger h = _mPSO->maxTotalThreadsPerThreadgroup() / w;
    MTL::Size threadsPerGrid        = MTL::Size::Make(IMAGE_WIDTH, IMAGE_HEIGHT, 1);
    MTL::Size threadsPerThreadgroup = MTL::Size::Make(w, h, 1);
    computeEncoder->dispatchThreads(threadsPerGrid, threadsPerThreadgroup);

    computeEncoder->endEncoding();

    //
    // Execute the command and wait for the GPU.
    //

    commandBuffer->commit();
    commandBuffer->waitUntilCompleted();

    //
    // On push back dans le buffer logiciel les resultats de calcul.
    //

    memcpy(ptr, _mBuffer->contents(), sizeof(float) * IMAGE_SIZE);

    pool->release();
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
bool MetalEngine::is_valid()
{
    if( _mPSO != nullptr )
        return true;
    return init();
}
#endif
