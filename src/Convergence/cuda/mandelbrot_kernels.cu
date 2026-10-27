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
#include "mandelbrot_kernels.hpp"

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
//
// Taille des blocs de threads : 16 x 16 = 256 threads (multiple de 32 = warp)
//
#define BLOCK_X 16
#define BLOCK_Y 16
/*
 *
 *
 *##############################################################################
 *
 *
 */
//
// Un seul kernel, instancie en float (SP) et en double (DP)
//
template <typename T>
__global__ void mandelbrot_kernel(
          float* __restrict__ v_dat,
    const T      zoom,
    const T      cornerX,
    const T      cornerY,
    const int    width,
    const int    height,
    const int    max_iters
)
{
    const int x = blockIdx.x * blockDim.x + threadIdx.x;
    const int y = blockIdx.y * blockDim.y + threadIdx.y;

    //
    // Si jamais notre position dans la grille déborde
    //
    if ( (x >= width) || (y >= height) )
    {
        return;
    }

    const T startReal = cornerX + zoom * (T)x;
    const T startImag = cornerY + zoom * (T)y;

    T zReal = startReal;
    T zImag = startImag;

    int counter = 0;
    for (; counter < max_iters; counter += 1)
    {
        const T r2 = zReal * zReal;
        const T i2 = zImag * zImag;

        if ( (r2 + i2) > (T)4 )
        {
            break;
        }

        zImag = (T)2 * zReal * zImag + startImag;
        zReal = r2 - i2 + startReal;
    }

    v_dat[y * width + x] = (float)counter;
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
template <typename T>
static void launch(float* v_dat, const T zoom, const T cornerX, const T cornerY, const int width, const int height, const int max_iters)
{
    const dim3 DimBlock(BLOCK_X, BLOCK_Y);
    const dim3 DimGrid ((width  + BLOCK_X - 1) / BLOCK_X,
                        (height + BLOCK_Y - 1) / BLOCK_Y);

    mandelbrot_kernel<T><<<DimGrid, DimBlock>>>(v_dat, zoom, cornerX, cornerY, width, height, max_iters);

    const cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess)
    {
        fprintf(stderr, "(EE) Failed to launch the mandelbrot kernel (error code %s)!\n", cudaGetErrorString(err));
        fprintf(stderr, "(EE) issue happened in file %s at line %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
void mandelbrot_sp_launch(float* v_dat, const float zoom, const float cornerX, const float cornerY, const int width, const int height, const int max_iters)
{
    launch<float>(v_dat, zoom, cornerX, cornerY, width, height, max_iters);
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
void mandelbrot_dp_launch(float* v_dat, const double zoom, const double cornerX, const double cornerY, const int width, const int height, const int max_iters)
{
    launch<double>(v_dat, zoom, cornerX, cornerY, width, height, max_iters);
}
#endif
