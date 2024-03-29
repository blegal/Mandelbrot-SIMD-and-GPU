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
#ifdef __SSE4_2__
#include "DP_SSE4_OMP.hpp"
//#include "vectclass/vectorclass.h"  // library for INTEL AVX2/AVX512 procesors

DP_SSE4_OMP::DP_SSE4_OMP() : Convergence("DP_SSE4_OMP")
{
    dataFormat  = "double";
    modeSIMD    = "SSE4";
    modeOPENMP  = "enable";
    OTHER       = "vectclass";
}

DP_SSE4_OMP::DP_SSE4_OMP(ColorMap *_colors, int _max_iters) : Convergence("DP_SSE4_OMP")
{
    colors      = _colors;
    max_iters   = _max_iters;

    dataFormat  = "double";
    modeSIMD    = "SSE4";
    modeOPENMP  = "enable";
    OTHER       = "vectclass";

}

DP_SSE4_OMP::~DP_SSE4_OMP()
{

}


void DP_SSE4_OMP::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
    const double f_zoom    = (double) _zoom;
    const double f_offsetX = (double) _offsetX;
    const double f_offsetY = (double) _offsetY;

    const Vec2d cste_1f(1.0f);
    const Vec2d cste_2f(2.0f);
    const Vec2d cste_4f(4.0f);

    const double sReal = f_offsetX - IMAGE_WIDTH  / 2.0 * f_zoom;
    const double sImag = f_offsetY - IMAGE_HEIGHT / 2.0 * f_zoom;

    const Vec2d XStep(2.0 * f_zoom);

#pragma omp parallel default (shared)// on declare une section parallel
    {
#pragma omp for // on fait du multicoeur
        for (int y = 0; y < IMAGE_HEIGHT; y++) {

            float* ptr_o = ptr + y * IMAGE_WIDTH;

            Vec2d v_startReal(sReal, sReal + f_zoom);
            const Vec2d v_startImag(sImag + (y * f_zoom));

            for (uint32_t x = 0; x < IMAGE_WIDTH; x += v_startReal.size()) {

                Vec2d v_value = 0.0f;
                Vec2d v_zReal = v_startReal;
                Vec2d v_zImag = v_startImag;

                for (uint32_t counter = 0; counter < max_iters; counter++) {
                    const Vec2d v_r2 = v_zReal * v_zReal;
                    const Vec2d v_i2 = v_zImag * v_zImag;

                    v_zImag = cste_2f * (v_zReal * v_zImag) + v_startImag;
                    v_zReal = (v_r2 - v_i2) + v_startReal;

                    const Vec2d v_r2_plus_i2 = (v_r2 + v_i2);

                    const Vec2db v_cmp_res = v_r2_plus_i2 < cste_4f; // Tout le monde > 4.0f
                    const Vec2d u_value    = v_value + cste_1f;
                    v_value                = select(v_cmp_res, u_value, v_value); // si r2+i2 < 4 value++

                    const uint32_t res = horizontal_or(v_cmp_res);
                    if (res == 0) { // si r2+i2 > 4 pour tous les élements
                        break;
                    }
                }

                double buffer[2];
                v_value.store(buffer);               // car c'est le format de sortie souhaité
                ptr_o[0] = buffer[0];
                ptr_o[1] = buffer[1];
                ptr_o   += v_value.size();          // On avance le pointeur

                v_startReal = v_startReal + XStep;
            }
        }
    }
}


bool DP_SSE4_OMP::is_valid()
{
    return true;
}
#endif