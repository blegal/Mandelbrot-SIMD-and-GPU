#include "SP_AVX2_u2.hpp"

#ifdef __AVX2__
    #include <immintrin.h>
#endif

#define SPP_CONV_STEP 16

SP_AVX2_u2::SP_AVX2_u2() : Convergence("SP_AVX2_u2")
{
    fractal     = "mandelbrot";
    dataFormat  = "float";
    modeSIMD    = "AVX2";
    modeOPENMP  = "disable";
    OTHER       = "unroll 2x";
}


SP_AVX2_u2::SP_AVX2_u2(ColorMap* _colors, int _max_iters) : Convergence("SP_AVX2_u2")
{
    colors      = _colors;
    max_iters   = _max_iters;

    fractal     = "mandelbrot";
    dataFormat  = "float";
    modeSIMD    = "AVX2";
    modeOPENMP  = "disable";
    OTHER       = "unroll 2x";

}


SP_AVX2_u2::~SP_AVX2_u2()
{

}


void SP_AVX2_u2::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
#ifdef __AVX2__

    const float f_zoom    = (float)_zoom;
    const float f_offsetX = (float)_offsetX;
    const float f_offsetY = (float)_offsetY;

    // consts :
    const __m256 v_4_0f = _mm256_set1_ps(4.0f);
    const __m256 v_2_0f = _mm256_set1_ps(2.0f);
    const __m256 v_1_0f = _mm256_set1_ps(1.0f);

        for (int y = 0 ; y < IMAGE_HEIGHT ; y++) {

            float startImag = f_offsetY - IMAGE_HEIGHT / 2.0f * f_zoom + (y * f_zoom);
            __attribute__ ((aligned (32))) float t_startImag[8] = {startImag, startImag, startImag, startImag, startImag, startImag, startImag, startImag};
            __m256 v_startImag = _mm256_loadu_ps(t_startImag);

            float startReal = f_offsetX - IMAGE_WIDTH / 2.0f * f_zoom;
            __attribute__ ((aligned (32))) float t_startReal_1[8] = {startReal       , startReal+f_zoom  , startReal+f_zoom*2 , startReal+f_zoom*3 , startReal+f_zoom*4 , startReal+f_zoom*5 , startReal+f_zoom*6 , startReal+f_zoom*7 };
            __m256 v_startReal_1 = _mm256_loadu_ps(t_startReal_1);
            __attribute__ ((aligned (32))) float t_startReal_2[8] = {startReal+f_zoom*8, startReal+f_zoom*9, startReal+f_zoom*10, startReal+f_zoom*11, startReal+f_zoom*12, startReal+f_zoom*13, startReal+f_zoom*14, startReal+f_zoom*15};
            __m256 v_startReal_2 = _mm256_loadu_ps(t_startReal_2);

            float* ptr_o = ptr + y * IMAGE_WIDTH;

            for (int x = 0 ; x < IMAGE_WIDTH ; x += SPP_CONV_STEP) {
                __m256 v_value_1 = _mm256_setzero_ps();
                __m256 v_value_2 = _mm256_setzero_ps();
                __m256 v_zReal_1 = v_startReal_1;
                __m256 v_zReal_2 = v_startReal_2;
                __m256 v_zImag_1 = v_startImag;
                __m256 v_zImag_2 = v_startImag;

                for (unsigned int counter = 0; counter < max_iters; counter++) {
                    __m256 v_r2_1 = _mm256_mul_ps(v_zReal_1, v_zReal_1);
                    __m256 v_i2_1 = _mm256_mul_ps(v_zImag_1, v_zImag_1);
                    __m256 v_r2_2 = _mm256_mul_ps(v_zReal_2, v_zReal_2);
                    __m256 v_i2_2 = _mm256_mul_ps(v_zImag_2, v_zImag_2);

                    v_zImag_1 = _mm256_add_ps(_mm256_mul_ps(v_2_0f, _mm256_mul_ps(v_zReal_1, v_zImag_1)), v_startImag);
                    v_zImag_2 = _mm256_add_ps(_mm256_mul_ps(v_2_0f, _mm256_mul_ps(v_zReal_2, v_zImag_2)), v_startImag);
                    v_zReal_1 = _mm256_add_ps(_mm256_sub_ps(v_r2_1, v_i2_1), v_startReal_1);
                    v_zReal_2 = _mm256_add_ps(_mm256_sub_ps(v_r2_2, v_i2_2), v_startReal_2);

                    __m256 v_r2_plus_i2_1 = _mm256_add_ps(v_r2_1, v_i2_1);
                    __m256 v_r2_plus_i2_2 = _mm256_add_ps(v_r2_2, v_i2_2);

                    __m256 v_cmp_res_1 = _mm256_cmp_ps(v_r2_plus_i2_1, v_4_0f, _CMP_LT_OS);
                    __m256 v_cmp_res_2 = _mm256_cmp_ps(v_r2_plus_i2_2, v_4_0f, _CMP_LT_OS);
                    v_value_1 = _mm256_blendv_ps(v_value_1, _mm256_add_ps(v_value_1, v_1_0f), v_cmp_res_1);
                    v_value_2 = _mm256_blendv_ps(v_value_2, _mm256_add_ps(v_value_2, v_1_0f), v_cmp_res_2);

                    short res_1 = _mm256_movemask_ps(v_cmp_res_1);
                    short res_2 = _mm256_movemask_ps(v_cmp_res_2);
                    if (res_1 == 0 && res_2 == 0) { // si r2+i2 > 4 pour tous les élements
                        break;
                    }
                }

                _mm256_storeu_ps(ptr_o, v_value_1); ptr_o += (sizeof(__m256)/sizeof(float));
                _mm256_storeu_ps(ptr_o, v_value_2); ptr_o += (sizeof(__m256)/sizeof(float));

                v_startReal_1 = _mm256_add_ps(v_startReal_1, _mm256_set1_ps(f_zoom*SPP_CONV_STEP));
                v_startReal_2 = _mm256_add_ps(v_startReal_2, _mm256_set1_ps(f_zoom*SPP_CONV_STEP));
            }
        }
#endif
}


bool SP_AVX2_u2::is_valid()
{
#ifdef __AVX2__
    return true;
#else
    return false;
#endif
}
