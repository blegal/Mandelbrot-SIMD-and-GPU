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
#include "DP_AVX512.hpp"

#if defined(__AVX512F__)
    #include "vectclass/vectorclass.h"
#endif

DP_AVX512::DP_AVX512() : Convergence("DP_AVX512")
{
    fractal     = "mandelbrot";
    dataFormat  = "double";
    modeSIMD    = "AVX512";
    modeOPENMP  = "disable";
    OTHER       = "none";
}

DP_AVX512::DP_AVX512(ColorMap *_colors, int _max_iters) : Convergence("DP_AVX512")
{
    colors      = _colors;
    max_iters   = _max_iters;

    fractal     = "mandelbrot";
    dataFormat  = "float";
    modeSIMD    = "AVX512";
    modeOPENMP  = "disable";
    OTHER       = "none";

}

DP_AVX512::~DP_AVX512()
{

}


#define SP_CONV_STEP 4

void DP_AVX512::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
#if defined(__AVX512BW__) || defined(__AVX2__)
    const float f_zoom    = (float) _zoom;
    const float f_offsetX = (float) _offsetX;
    const float f_offsetY = (float) _offsetY;

    // consts :
    const Vec16f cste_1f(1.0f);
    const Vec16f cste_2f(2.0f);
    const Vec16f cste_4f(4.0f);

    const float startR = f_offsetX - IMAGE_WIDTH / 2.0f * f_zoom;
    const Vec16f startReal( startR + f_zoom *  0.0f, startR + f_zoom *  1.0f, startR + f_zoom *  2.0f, startR + f_zoom *  3.0f,
                            startR + f_zoom *  4.0f, startR + f_zoom *  5.0f, startR + f_zoom *  6.0f, startR + f_zoom *  7.0f,
                            startR + f_zoom *  8.0f, startR + f_zoom *  9.0f, startR + f_zoom * 10.0f, startR + f_zoom * 11.0f,
                            startR + f_zoom * 12.0f, startR + f_zoom * 13.0f, startR + f_zoom * 14.0f, startR + f_zoom * 15.0f);
    const Vec16f XStep(f_zoom * startReal.size());

        for (int y = 0; y < IMAGE_HEIGHT; y++) {

            float* ptr_o = ptr + y * IMAGE_WIDTH;

            const float startImag = f_offsetY - IMAGE_HEIGHT / 2.0f * f_zoom + (y * f_zoom);
            Vec16f v_startReal = startReal;
            const Vec16f v_startImag(startImag);

            for (uint32_t x = 0; x < IMAGE_WIDTH; x += v_startReal.size()) {

                Vec16f v_value = 0.0f;
                Vec16f v_zReal = v_startReal;
                Vec16f v_zImag = v_startImag;

                for (uint32_t counter = 0; counter < max_iters; counter++) {
                    const Vec16f v_r2 = v_zReal * v_zReal;
                    const Vec16f v_i2 = v_zImag * v_zImag;

                    v_zImag = cste_2f * (v_zReal * v_zImag) + v_startImag;
                    v_zReal = (v_r2 - v_i2) + v_startReal;

                    const Vec16f v_r2_plus_i2 = (v_r2 + v_i2);

                    const Vec16fb v_cmp_res = v_r2_plus_i2 <= cste_4f; // Tout le monde > 4.0f

                    const Vec16f u_value = v_value + cste_1f;
                    v_value = select(v_cmp_res, u_value, v_value); // si r2+i2 < 4 value++

                    const uint16_t res = horizontal_or(v_cmp_res);
                    if (res == 0) { // si r2+i2 > 4 pour tous les élements
                        break;
                    }
                }

                v_value.store(ptr_o);
                ptr_o += v_value.size();

                v_startReal = v_startReal + XStep;
            }
    }
#endif
}


bool DP_AVX512::is_valid()
{
#if defined(__AVX512BW__) || defined(__AVX2__)
    return true;
#else
    return false;
#endif
}
