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
#include "DP_x86_M.hpp"

// Multibrot (d=3)

DP_x86_M::DP_x86_M() : Convergence("DP_x86_M", "Multibrot (3)")
{
    dataFormat  = "double";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}


DP_x86_M::DP_x86_M(ColorMap* _colors, int _max_iters) : Convergence("DP_x86_M", "Multibrot (3)")
{
    colors      = _colors;
    max_iters   = _max_iters;

    dataFormat  = "double";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}


DP_x86_M::~DP_x86_M( ){

}


void DP_x86_M::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
    double offsetX = _offsetX;
    double offsetY = _offsetY;
    double zoom    = _zoom;

    for (int y = 0; y < IMAGE_HEIGHT; y++) {

        double startImag = offsetY - IMAGE_HEIGHT / 2.0f * zoom + (y * zoom);
        double startReal = offsetX - IMAGE_WIDTH  / 2.0f * zoom;

        for (int x = 0; x < IMAGE_WIDTH;  x++) {
            int value    = max_iters - 1;
            double zReal = startReal;
            double zImag = startImag;

            for (int counter = 0; counter < max_iters; counter++) {
                ////////////////////////////////////////////////////////////////////////////////////////////////////////
                double r2 = zReal * zReal;
                double r3 = zReal * zReal * zReal;
                double i2 = zImag * zImag;
                double i3 = zImag * zImag * zImag;
                zImag = 3.0f * r2 * zImag - i3 + startImag;
                zReal = r3 - 3.0f * zReal * i2 + startReal;
                if ( (r2 + i2) > 4.0f) {
                    value = counter;
                    break;
                }
                ////////////////////////////////////////////////////////////////////////////////////////////////////////
            }
            (*ptr++) = value;
            startReal += zoom;
        }
    }
}


bool DP_x86_M::is_valid()
{
    return true;
}

