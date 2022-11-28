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
#include "SP_x86.hpp"

SP_x86::SP_x86() : Convergence("SP")
{
    fractal     = "mandelbrot";
    dataFormat  = "float";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}


SP_x86::SP_x86(ColorMap* _colors, int _max_iters) : Convergence("SP") {
    colors      = _colors;
    max_iters   = _max_iters;

    fractal     = "mandelbrot";
    dataFormat  = "float";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}


SP_x86::~SP_x86( )
{

}


unsigned int SP_x86::process(const float startReal, const float startImag, unsigned int max_iters)
{
    float zReal = startReal;
    float zImag = startImag;

    for (int counter = 0; counter < max_iters; counter++) {
        float r2 = zReal * zReal;
        float i2 = zImag * zImag;
        zImag = 2.0f * zReal * zImag + startImag;
        zReal = r2 - i2 + startReal;
        if ( (r2 + i2) > 4.0f) {
            return counter;
        }
    }
    return max_iters - 1;
}


void SP_x86::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
    const float f_zoom    = (float)_zoom;
    const float f_offsetX = (float)_offsetX;
    const float f_offsetY = (float)_offsetY;

#if 0
    float f_coin_x = f_offsetY  -  (IMAGE_HEIGHT/2) * f_zoom;
    float f_coin_y = f_offsetX  -  (IMAGE_WIDTH/2 ) * f_zoom;
    printf("(DD) Position de la fenetre (SP_x86)\n");
    printf("(DD) - CENTRE : X = %1.13f | Y = %1.13f | S = %1.13f\n", f_offsetX, f_offsetY, f_zoom);
    printf("(DD) - COIN   : X = %1.13f | Y = %1.13f | S = %1.13f\n", f_coin_x,   f_coin_y, f_zoom);
#endif

    for (int y = 0; y < IMAGE_HEIGHT; y++)
    {
        float imag = f_offsetY - IMAGE_HEIGHT / 2.0f * f_zoom + (y * f_zoom);
        float real = f_offsetX - IMAGE_WIDTH  / 2.0f * f_zoom;

        for (int x = 0; x < IMAGE_WIDTH;  x++)
        {
            uint32_t value = process(real, imag, max_iters);
            (*ptr++) = value;
            real += f_zoom;
        }
    }
}


bool SP_x86::is_valid()
{
    return true;
}
