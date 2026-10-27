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
#include "SP_metal.hpp"
#include <cstdint>
/*
 *
 *
 *##############################################################################
 *
 *
 */
//
// Doit correspondre a la structure sp_params de mandelbrot.metal
//
struct sp_params {
    float    zoom;
    float    cornerX;
    float    cornerY;
    uint32_t width;
    uint32_t height;
    uint32_t max_iters;
};
/*
 *
 *
 *##############################################################################
 *
 *
 */
SP_metal::SP_metal() : MetalEngine("SP_METAL", "mandelbrot_sp", true)
{
    dataFormat  = "float";
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
SP_metal::SP_metal(ColorMap* _colors, int _max_iters) : MetalEngine("SP_METAL", "mandelbrot_sp", true)
{
    colors      = _colors;
    max_iters   = _max_iters;
    dataFormat  = "float";
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
    const long double corner_x = _offsetX - (IMAGE_WIDTH  / 2.0L) * _zoom;
    const long double corner_y = _offsetY - (IMAGE_HEIGHT / 2.0L) * _zoom;

    sp_params params;
    params.zoom      = (float)_zoom;
    params.cornerX   = (float)corner_x;
    params.cornerY   = (float)corner_y;
    params.width     = IMAGE_WIDTH;
    params.height    = IMAGE_HEIGHT;
    params.max_iters = max_iters;

    run(&params, sizeof(params), IMAGE_WIDTH, IMAGE_HEIGHT, ptr);
}
#endif
