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
#include "DP_x86_OMP.hpp"

DP_x86_OMP::DP_x86_OMP() : Convergence("DP_x86_OMP", "Mandelbrot")
{
    dataFormat  = "double";
    modeSIMD    = "none";
    modeOPENMP  = "enable";
    OTHER       = "none";
}

DP_x86_OMP::DP_x86_OMP(ColorMap* _colors, int _max_iters) : Convergence("DP_x86_OMP", "Mandelbrot")
{
    colors      = _colors;
    max_iters   = _max_iters;

    dataFormat  = "double";
    modeSIMD    = "none";
    modeOPENMP  = "enable";
    OTHER       = "none";
}

DP_x86_OMP::~DP_x86_OMP( )
{

}

unsigned int DP_x86_OMP::process(const double startReal, const double startImag, unsigned int max_iters)
{
    double zReal = startReal;
    double zImag = startImag;

    for (int counter = 0; counter < max_iters; counter++) {
        double r2 = zReal * zReal;
        double i2 = zImag * zImag;
        zImag = 2.0f * zReal * zImag + startImag;
        zReal = r2 - i2 + startReal;
        if ( (r2 + i2) > 4.0f) {
            return counter;
        }
    }
    return max_iters - 1;
}


void DP_x86_OMP::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
    const double offsetX = _offsetX;
    const double offsetY = _offsetY;
    const double zoom    = _zoom;

#pragma omp parallel // on declare une section parallel
    {
#pragma omp for // on fait du multicoeur
        for (int y = 0; y < IMAGE_HEIGHT; y++) {

            float* ptr_o = ptr + y * IMAGE_WIDTH;

            double imag = offsetY - IMAGE_HEIGHT / 2.0f * zoom + (y * zoom);
            double real = offsetX - IMAGE_WIDTH  / 2.0f * zoom;

            for (int x = 0; x < IMAGE_WIDTH;  x++) {
                int value = process(real, imag, max_iters);
                (*ptr_o++) = value;
                real += zoom;
            }
        }
    }
}


bool DP_x86_OMP::is_valid()
{
    return true;
}

