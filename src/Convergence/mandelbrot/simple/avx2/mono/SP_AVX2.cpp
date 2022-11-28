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
#ifdef __AVX2__
#include "SP_AVX2.hpp"
#include <immintrin.h>

#define SPP_CONV_STEP 16

SP_AVX2::SP_AVX2() : Convergence("SP_AVX2")
{
    fractal     = "mandelbrot";
    dataFormat  = "float";
    modeSIMD    = "AVX2";
    modeOPENMP  = "disable";
    OTHER       = "none";
}

SP_AVX2::SP_AVX2(ColorMap *_colors, int _max_iters) : Convergence("SP_AVX2")
{
    colors      = _colors;
    max_iters   = _max_iters;

    fractal     = "mandelbrot";
    dataFormat  = "float";
    modeSIMD    = "AVX2";
    modeOPENMP  = "disable";
    OTHER       = "none";

}

SP_AVX2::~SP_AVX2() {

}


void SP_AVX2::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
    const float f_zoom    = (float)_zoom;
    const float f_offsetX = (float)_offsetX;
    const float f_offsetY = (float)_offsetY;

    // consts :
    const __m256 v_4_0f = _mm256_set1_ps(4.0f);
    const __m256 v_2_0f = _mm256_set1_ps(2.0f);
    const __m256 v_1_0f = _mm256_set1_ps(1.0f);

        for (int y = 0; y < IMAGE_HEIGHT; y++) {

            float startImag = f_offsetY - IMAGE_HEIGHT / 2.0f * f_zoom + (y * f_zoom);
            __attribute__ ((aligned (32))) float t_startImag[8] = {startImag, startImag, startImag, startImag,
                                                                   startImag, startImag, startImag, startImag};
            __m256 v_startImag = _mm256_loadu_ps(t_startImag);

            float startReal = f_offsetX - IMAGE_WIDTH / 2.0f * f_zoom;
            __attribute__ ((aligned (32))) float t_startReal[8] = {startReal, startReal + f_zoom,
                                                                   startReal + f_zoom * 2, startReal + f_zoom * 3,
                                                                   startReal + f_zoom * 4, startReal + f_zoom * 5,
                                                                   startReal + f_zoom * 5, startReal + f_zoom * 6};
            __m256 v_startReal = _mm256_loadu_ps(t_startReal);

            float* ptr_o = ptr + y * IMAGE_WIDTH;

            for (int x = 0; x < IMAGE_WIDTH; x += SPP_CONV_STEP) {
                __m256 v_value = _mm256_setzero_ps();
                __m256 v_zReal = v_startReal;
                __m256 v_zImag = v_startImag;

                for (int counter = 0; counter < max_iters; counter++) {
                    __m256 v_r2 = _mm256_mul_ps(v_zReal, v_zReal);
                    __m256 v_i2 = _mm256_mul_ps(v_zImag, v_zImag);

                    v_zImag = _mm256_add_ps(_mm256_mul_ps(v_2_0f, _mm256_mul_ps(v_zReal, v_zImag)), v_startImag);
                    v_zReal = _mm256_add_ps(_mm256_sub_ps(v_r2, v_i2), v_startReal);

                    __m256 v_r2_plus_i2 = _mm256_add_ps(v_r2, v_i2);
                    __m256 v_cmp_res = _mm256_cmp_ps(v_r2_plus_i2, v_4_0f, _CMP_LT_OS);
                    v_value = _mm256_blendv_ps(v_value, _mm256_add_ps(v_value, v_1_0f),
                                               v_cmp_res); // si r2+i2 < 4 value++

                    const int32_t res = _mm256_movemask_ps(v_cmp_res);
                    if (res == 0) { // si r2+i2 > 4 pour tous les élements
                        break;
                    }
                }

                _mm256_storeu_ps(ptr_o, v_value);
                ptr_o += (sizeof(__m256)/sizeof(float));

                v_startReal = _mm256_add_ps(v_startReal, _mm256_set1_ps(f_zoom * SPP_CONV_STEP));
            }
        }
}


bool SP_AVX2::is_valid()
{
    return true;
}
#endif