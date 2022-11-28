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
#include "FP_Q18_14_x86.hpp"

FP_Q18_14_x86::FP_Q18_14_x86() : Convergence("FP_Q18_14_x86")
{
    dataFormat  = "fixed";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}

FP_Q18_14_x86::FP_Q18_14_x86(ColorMap* _colors, int _max_iters) : Convergence("FP_Q18_14_x86")
{
    colors      = _colors;
    max_iters   = _max_iters;

    dataFormat  = "fixed";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}

FP_Q18_14_x86::~FP_Q18_14_x86()
{

}

#define Q_FORMAT   FI_18_15
#define Q_SATURE   MAX_FI_18

void FP_Q18_14_x86::updateImage(const long double d_zoom, const long double d_offsetX, const long double d_offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
    const fi32_t offsetX = double_to_fi32(d_offsetX, Q_FORMAT);
    const fi32_t offsetY = double_to_fi32(d_offsetY, Q_FORMAT);
    const fi32_t zoom    = double_to_fi32(d_zoom,    Q_FORMAT);

#if 0
    fi32_t coin_x = offsetY  -  (IMAGE_HEIGHT/2) * zoom;
    fi32_t coin_y = offsetX  -  (IMAGE_WIDTH/2 ) * zoom;
    printf("(DD) Position de la fenetre §(FP_Q18_15_x86)\n");
    printf("(DD) - CENTRE : X = %12d | Y = %12d | S = %12d\n", offsetX, offsetY, zoom);
    printf("(DD) - COIN   : X = %12d | Y = %12d | S = %12d\n", coin_x,   coin_y, zoom);
#endif

    for (int y = 0; y < IMAGE_HEIGHT; y++)
    {
        fi32_t startImag = offsetY  -  IMAGE_HEIGHT/2 * zoom  +  y * zoom;
        fi32_t startReal = offsetX  -  IMAGE_WIDTH/2  * zoom;

        for (int x = 0; x < IMAGE_WIDTH;  x++) {

            int value = max_iters - 1;

            fi32_t zReal = startReal;
            fi32_t zImag = startImag;

            for (int counter = 0; counter < max_iters; counter++)
            {
                //double d_r2 = d_zReal * d_zReal;
                const fi64_t _r2_64 = ((fi64_t)(zReal)) * ((fi64_t)(zReal));
                const fi64_t _r2_   = _r2_64 >> Q_FORMAT;
                const fi32_t _r2   = (_r2_ < Q_SATURE) ? _r2_ : Q_SATURE;  // USELESS ?

                //double d_i2 = d_zImag * d_zImag;
                const fi64_t _i2_64 = (((fi64_t)(zImag)) * ((fi64_t)(zImag)));
                const fi64_t _i2_   = _i2_64 >> Q_FORMAT;
                const fi32_t _i2   = (_i2_ < Q_SATURE) ? _i2_ : Q_SATURE;   // USELESS ?

                //double d_i2 = d_zReal * d_zImag;
                const fi64_t _ri_64 = (((fi64_t)(zReal)) * ((fi64_t)(zImag)));
                const fi64_t _ri_   = _ri_64 >> Q_FORMAT;
                const fi32_t _ri   = (_ri_ < Q_SATURE) ? _ri_ : Q_SATURE;   // USELESS ?
                const fi32_t _2ri  = 2 * _ri;

                zImag = (_2ri      + startImag);
                zReal = (_r2 - _i2 + startReal);

                if ( (_r2 + _i2) > (4 << Q_FORMAT))
                {
                    value = counter;
                    break;
                }
            }
            (*ptr++) = value;
            startReal = startReal + zoom;
        }
    }
}


bool FP_Q18_14_x86::is_valid()
{
    return true;
}
