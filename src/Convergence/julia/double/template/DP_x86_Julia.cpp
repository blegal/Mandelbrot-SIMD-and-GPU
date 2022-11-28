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
#include "DP_x86_Julia.hpp"


//DP_x86_Julia::DP_x86_Julia() : Convergence("DP_x86_Julia", "Julia", const double jReal, const double jImag)
//    : julia_Real(jReal), julia_Imag( jImag )
//{
//    dataFormat  = "double";
//    modeSIMD    = "none";
//    modeOPENMP  = "disable";
//    OTHER       = "none";
//}


DP_x86_Julia::DP_x86_Julia(
        const std::string name,
        const std::string fract,
        ColorMap* _colors,
        const int _max_iters,
        const double jReal,
        const double jImag) :
    Convergence(name, fract), julia_Real(jReal), julia_Imag( jImag )
{
    colors      = _colors;
    max_iters   = _max_iters;

    dataFormat  = "double";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}


DP_x86_Julia::~DP_x86_Julia( ){

}


void DP_x86_Julia::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
    double corner_x = _offsetX - (IMAGE_WIDTH  / 2.0) * _zoom;
    double corner_y = _offsetY - (IMAGE_HEIGHT / 2.0) * _zoom;
    double zoom     = _zoom;

    const double startReal = julia_Real;
    const double startImag = julia_Imag;

    for (int y = 0; y < IMAGE_HEIGHT; y++)
    {
        const double ref_zImag = corner_y + ((double)y * zoom);   //yn

        for (int x = 0; x < IMAGE_WIDTH;  x++)
        {
            int value    = max_iters - 1;

            double zReal = corner_x + ((double)x * zoom);   //xn
            double zImag = ref_zImag;

//          https://fr.wikipedia.org/wiki/Ensemble_de_Julia#/media/Fichier:Julia_set_(Detail_01).jpg

            for (int counter = 0; counter < max_iters; counter++)
            {
                ////////////////////////////////////////////////////////////////////////////////////////////////////////
                double r2 = zReal * zReal;
                double i2 = zImag * zImag;
                zImag = 2.0 * zReal * zImag + startImag;   //yn
                zReal = (r2 - i2) + startReal;              //xn
                if ( (r2 + i2) > 4.0f) {
                    value = counter;
                    break;
                }
                ////////////////////////////////////////////////////////////////////////////////////////////////////////
            }
            (*ptr++) = value;
        }
    }
}


bool DP_x86_Julia::is_valid()
{
    return true;
}

