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
#include "DP_cuda.hpp"
#include "Convergence/cuda/mandelbrot_kernels.hpp"
/*
 *
 *
 *##############################################################################
 *
 *
 */
DP_cuda::DP_cuda() : CudaEngine("DP_CUDA")
{
    dataFormat  = "double";
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
DP_cuda::DP_cuda(ColorMap* _colors, int _max_iters) : CudaEngine("DP_CUDA")
{
    colors      = _colors;
    max_iters   = _max_iters;
    dataFormat  = "double";
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
DP_cuda::~DP_cuda( )
{

}
/*
 *
 *
 *##############################################################################
 *
 *
 */
void DP_cuda::updateImage(
        const long double _zoom,
        const long double _offsetX,
        const long double _offsetY,
        const int IMAGE_WIDTH,
        const int IMAGE_HEIGHT,
        float* ptr)
{
    const size_t IMAGE_SIZE = (size_t)IMAGE_WIDTH * IMAGE_HEIGHT;

    const long double corner_x = _offsetX - (IMAGE_WIDTH  / 2.0L) * _zoom;
    const long double corner_y = _offsetY - (IMAGE_HEIGHT / 2.0L) * _zoom;

    float* gpu_mBuffer = reserve( IMAGE_SIZE );

    mandelbrot_dp_launch(
        gpu_mBuffer,
        (double)_zoom,
        (double)corner_x,
        (double)corner_y,
        IMAGE_WIDTH,
        IMAGE_HEIGHT,
        max_iters
    );

    download(ptr, IMAGE_SIZE);
}
#endif
