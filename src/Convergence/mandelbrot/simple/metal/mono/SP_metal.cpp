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
#if defined(__APPLE__)
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
#include "SP_metal.hpp"
/*
 *
 *
 *##############################################################################
 *
 *
 */
SP_metal::SP_metal() : Convergence("SP_METAL")
{
    fractal     = "mandelbrot";
    dataFormat  = "float";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "metal";

    _mBuffer    = nullptr;
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
SP_metal::SP_metal(ColorMap* _colors, int _max_iters) : Convergence("SP_METAL") {
    colors      = _colors;
    max_iters   = _max_iters;

    fractal     = "mandelbrot";
    dataFormat  = "float";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";

    _mBuffer    = nullptr;



}
/*
 *
 *
 *##############################################################################
 *
 *
 */
SP_metal::~SP_metal( )
{

}
/*
 *
 *
 *##############################################################################
 *
 *
 */
void SP_metal::updateImage(
        const long double _zoom,
        const long double _offsetX,
        const long double _offsetY,
        const int IMAGE_WIDTH,
        const int IMAGE_HEIGHT,
        float* ptr)
{
    const int IMAGE_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT;

    if( _mBuffer == nullptr )
    {
        //
        // On recherche la device par defaut dans le systeme
        //

        _mDevice = MTL::CreateSystemDefaultDevice();

        NS::Error *error = nullptr;

        //
        // On cherche la bibliotheque contenant le code metal compilé
        //

        NS::String* filePath         = NS::String::string("./mandelbrot_sp.metallib", NS::UTF8StringEncoding);
        MTL::Library *defaultLibrary = _mDevice->newLibrary(filePath, &error);
        if (defaultLibrary == nullptr)
        {
            std::cout << "(EE) Failed to find the [mandelbrot_sp.metallib] library file." << std::endl;
            exit( EXIT_FAILURE );
        }

        //
        // On part à la recherche de la fonction (metal) dans la bibliotheque
        //

        auto str = NS::String::string("mandelbrot_sp",       NS::ASCIIStringEncoding);
        MTL::Function *addFunction = defaultLibrary->newFunction(str);
        if (addFunction == nullptr)
        {
            std::cout << "(EE) Failed to find the [mandelbrot_sp] function in the library." << std::endl;
            exit( EXIT_FAILURE );
        }

        //
        // On cree les structures qui vont nous permettre de lancer les operations sur le GPU
        //

        _mAddFunctionPSO = _mDevice->newComputePipelineState(addFunction, &error);
        if (_mAddFunctionPSO == nullptr)
        {
            //  If the Metal API validation is enabled, you can find out more information about what
            //  went wrong.  (Metal API validation is enabled by default when a debug build is run
            //  from Xcode)
            std::cout << "Failed to created pipeline state object, error " << error << "." << std::endl;
            exit( EXIT_FAILURE );
        }

        _mCommandQueue = _mDevice->newCommandQueue();
        if (_mCommandQueue == nullptr)
        {
            std::cout << "Failed to find the command queue." << std::endl;
            exit( EXIT_FAILURE );
        }

        //
        //  Allocation des zones mémoire permettant d'échanger avec le GPU
        //

        _mBuffer  = _mDevice->newBuffer(sizeof(uint32_t) * IMAGE_SIZE, MTL::ResourceStorageModeShared);
        _mZoom    = _mDevice->newBuffer(sizeof(float),    MTL::ResourceStorageModeShared);
        _mOffsetX = _mDevice->newBuffer(sizeof(float),    MTL::ResourceStorageModeShared);
        _mOffsetY = _mDevice->newBuffer(sizeof(float),    MTL::ResourceStorageModeShared);
        _mWidth   = _mDevice->newBuffer(sizeof(uint32_t), MTL::ResourceStorageModeShared);
        _mHeight  = _mDevice->newBuffer(sizeof(uint32_t), MTL::ResourceStorageModeShared);
        _mIters   = _mDevice->newBuffer(sizeof(uint32_t), MTL::ResourceStorageModeShared);
    }

    //
    // Create a command buffer to hold commands
    //

    MTL::CommandBuffer *commandBuffer = _mCommandQueue->commandBuffer();
    if (commandBuffer == nullptr)
    {
        std::cout << "(EE) Failed to create a command buffer." << std::endl;
        exit( EXIT_FAILURE );
    }

    //
    // Start a compute pass.
    //

    MTL::ComputeCommandEncoder *computeEncoder = commandBuffer->computeCommandEncoder();
    if (computeEncoder == nullptr)
    {
        std::cout << "(EE) Failed to create a compute encoder." << std::endl;
        exit( EXIT_FAILURE );
    }

    // Encode the pipeline state object and its parameters.
    computeEncoder->setComputePipelineState(_mAddFunctionPSO);

    const long double corner_x = _offsetX - (IMAGE_WIDTH  / 2.0f) * _zoom;
    const long double corner_y = _offsetY - (IMAGE_HEIGHT / 2.0f) * _zoom;

    printf("corner_x  = %f\n", corner_x);
    printf("corner_y  = %f\n", corner_y);
    printf("zoom      = %f\n", (float)_zoom);
    printf("max_iters = %d\n", max_iters);

    *((float*   )_mZoom   ->contents()) = _zoom;
    *((float*   )_mOffsetX->contents()) = corner_x;
    *((float*   )_mOffsetY->contents()) = corner_y;
    *((uint32_t*)_mWidth  ->contents()) = IMAGE_WIDTH;
    *((uint32_t*)_mHeight ->contents()) = IMAGE_HEIGHT;
    *((uint32_t*)_mIters  ->contents()) = max_iters;

    computeEncoder->setBuffer(_mBuffer,  0, 0);
    computeEncoder->setBuffer(_mZoom,    0, 1);
    computeEncoder->setBuffer(_mOffsetX, 0, 2);
    computeEncoder->setBuffer(_mOffsetY, 0, 3);
    computeEncoder->setBuffer(_mWidth,   0, 4);
    computeEncoder->setBuffer(_mHeight,  0, 5);
    computeEncoder->setBuffer(_mIters,   0, 6);

    const int w = _mAddFunctionPSO->threadExecutionWidth();
    const int h = _mAddFunctionPSO->maxTotalThreadsPerThreadgroup() / w;
    MTL::Size threadsPerGrid        = MTL::Size::Make(IMAGE_WIDTH, IMAGE_HEIGHT, 1);
    MTL::Size threadsPerThreadgroup = MTL::Size::Make(w, h, 1);
    computeEncoder->dispatchThreads(threadsPerGrid, threadsPerThreadgroup);

    // End the compute pass.
    computeEncoder->endEncoding();

    // Execute the command.
    commandBuffer->commit();

    // Normally, you want to do other work in your app while the GPU is running,
    // but in this example, the code simply blocks until the calculation is complete.
    commandBuffer->waitUntilCompleted();

    // On push back dans le buffer logiciel les resultats de calcul.

    const uint32_t* mBuffer = (uint32_t*)_mBuffer->contents();
    for (int p = 0; p < IMAGE_SIZE; p += 1)
    {
        ptr[p] = mBuffer[p];
    }
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
bool SP_metal::is_valid()
{
    return true;
}
#endif