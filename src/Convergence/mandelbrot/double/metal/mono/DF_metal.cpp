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
#include "DF_metal.hpp"
#include <cstdint>
/*
 *
 *
 *##############################################################################
 *
 *
 */
//
// Doit correspondre a la structure df_params de mandelbrot.metal
//
struct alignas(8) df_params { // float2 est aligne sur 8 octets cote Metal
    float    zoom   [2];
    float    cornerX[2];
    float    cornerY[2];
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
//
// Decoupe une valeur (long) double en deux floats : value ~= hi + lo
//
static void split(const long double value, float* dst)
{
    dst[0] = (float)value;
    dst[1] = (float)(value - (long double)dst[0]);
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
DF_metal::DF_metal() : MetalEngine("DF_METAL", "mandelbrot_df", false)
{
    dataFormat  = "df64";
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
DF_metal::DF_metal(ColorMap* _colors, int _max_iters) : MetalEngine("DF_METAL", "mandelbrot_df", false)
{
    colors      = _colors;
    max_iters   = _max_iters;
    dataFormat  = "df64";
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
DF_metal::~DF_metal( )
{

}
/*
 *
 *
 *##############################################################################
 *
 *
 */
void DF_metal::updateImage(
        const long double _zoom,
        const long double _offsetX,
        const long double _offsetY,
        const int IMAGE_WIDTH,
        const int IMAGE_HEIGHT,
        float* ptr)
{
    const long double corner_x = _offsetX - (IMAGE_WIDTH  / 2.0L) * _zoom;
    const long double corner_y = _offsetY - (IMAGE_HEIGHT / 2.0L) * _zoom;

    df_params params;
    split(_zoom,    params.zoom   );
    split(corner_x, params.cornerX);
    split(corner_y, params.cornerY);
    params.width     = IMAGE_WIDTH;
    params.height    = IMAGE_HEIGHT;
    params.max_iters = max_iters;

    run(&params, sizeof(params), IMAGE_WIDTH, IMAGE_HEIGHT, ptr);
}
#endif
