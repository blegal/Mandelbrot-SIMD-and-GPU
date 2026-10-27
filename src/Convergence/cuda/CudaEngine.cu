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
#if defined(_ENABLE_CUDA_)
/*
 *
 *
 *##############################################################################
 *
 *
 */
#include "CudaEngine.hpp"

#include <cuda_runtime.h>
#include <cstdio>
#include <cstdlib>
/*
 *
 *
 *##############################################################################
 *
 *
 */
#define CUDA_CHECK(call)                                                            \
    do {                                                                            \
        const cudaError_t err = (call);                                             \
        if (err != cudaSuccess) {                                                   \
            fprintf(stderr, "(EE) CUDA error (%s) in file %s at line %d\n",         \
                    cudaGetErrorString(err), __FILE__, __LINE__);                   \
            exit(EXIT_FAILURE);                                                     \
        }                                                                           \
    } while (0)
/*
 *
 *
 *##############################################################################
 *
 *
 */
CudaEngine::CudaEngine(const std::string _name_) : Convergence(_name_)
{
    fractal     = "mandelbrot";
    modeSIMD    = "GPU";
    modeOPENMP  = "disable";
    OTHER       = "CUDA";

    gpu_mBuffer = nullptr;
    gpu_mSize   = 0;
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
CudaEngine::~CudaEngine( )
{
    if( gpu_mBuffer != nullptr )
        cudaFree( gpu_mBuffer );
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
float* CudaEngine::reserve(const size_t IMAGE_SIZE)
{
    if( gpu_mSize != IMAGE_SIZE )
    {
        if( gpu_mBuffer != nullptr )
            CUDA_CHECK( cudaFree( gpu_mBuffer ) );
        CUDA_CHECK( cudaMalloc((void **)&gpu_mBuffer, IMAGE_SIZE * sizeof(float)) );
        gpu_mSize = IMAGE_SIZE;
    }
    return gpu_mBuffer;
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
void CudaEngine::download(float* ptr, const size_t IMAGE_SIZE)
{
    // cudaMemcpy attend implicitement la fin du kernel
    CUDA_CHECK( cudaMemcpy(ptr, gpu_mBuffer, IMAGE_SIZE * sizeof(float), cudaMemcpyDeviceToHost) );
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
bool CudaEngine::is_valid()
{
    int count = 0;
    if( cudaGetDeviceCount( &count ) != cudaSuccess )
        return false;
    return (count > 0);
}
#endif
