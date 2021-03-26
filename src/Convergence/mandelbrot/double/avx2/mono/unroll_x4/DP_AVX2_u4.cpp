#include "DP_AVX2_u4.hpp"

#ifdef __AVX2__
    #include <immintrin.h>
#endif

#define DPPP_CONV_STEP 16

DP_AVX2_u4::DP_AVX2_u4()  : Convergence("DP_AVX2_u4")
{
    dataFormat  = "double";
    modeSIMD    = "AVX2";
    modeOPENMP  = "disable";
    OTHER       = "unroll 4x";
}


DP_AVX2_u4::DP_AVX2_u4(ColorMap* _colors, int _max_iters)  : Convergence("DP_AVX2_u4")
{
    colors      = _colors;
    max_iters   = _max_iters;

    dataFormat  = "double";
    modeSIMD    = "AVX2";
    modeOPENMP  = "disable";
    OTHER       = "unroll 4x";
}


DP_AVX2_u4::~DP_AVX2_u4()
{

}


void DP_AVX2_u4::updateImage(
        const long double _zoom, const long double _offsetX, const long double _offsetY,
        const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
#ifdef __AVX2__
    const double offsetX = _offsetX;
    const double offsetY = _offsetY;
    const double zoom    = _zoom;

    const __m256d v_4_0f = _mm256_set1_pd(4.0f);
    const __m256d v_2_0f = _mm256_set1_pd(2.0f);
    const __m256d v_1_0f = _mm256_set1_pd(1.0f);

    const double sReal = offsetX - IMAGE_WIDTH  / 2.0 * zoom;
    const double sImag = offsetY - IMAGE_HEIGHT / 2.0 * zoom;

//#pragma omp parallel
    {
//#pragma omp for
        for (int y = 0 ; y < IMAGE_HEIGHT ; y++) {

            float* ptr_o = ptr + y * IMAGE_WIDTH;
                  __m256d v_startReal_1 = _mm256_setr_pd(sReal +  0.0 * zoom, sReal +  1.0 * zoom, sReal +  2.0 * zoom, sReal +  3.0 * zoom);
                  __m256d v_startReal_2 = _mm256_setr_pd(sReal +  4.0 * zoom, sReal +  5.0 * zoom, sReal +  6.0 * zoom, sReal +  7.0 * zoom);
                  __m256d v_startReal_3 = _mm256_setr_pd(sReal +  8.0 * zoom, sReal +  9.0 * zoom, sReal + 10.0 * zoom, sReal + 11.0 * zoom);
                  __m256d v_startReal_4 = _mm256_setr_pd(sReal + 12.0 * zoom, sReal + 13.0 * zoom, sReal + 14.0 * zoom, sReal + 15.0 * zoom);
            const __m256d v_startImag   = _mm256_set1_pd(sImag + (y * zoom));

            for (int x = 0 ; x < IMAGE_WIDTH ; x += DPPP_CONV_STEP) {

                __m256d v_value_1 = _mm256_setzero_pd();
                __m256d v_value_2 = _mm256_setzero_pd();
                __m256d v_value_3 = _mm256_setzero_pd();
                __m256d v_value_4 = _mm256_setzero_pd();

                __m256d v_zReal_1 = v_startReal_1;
                __m256d v_zReal_2 = v_startReal_2;
                __m256d v_zReal_3 = v_startReal_3;
                __m256d v_zReal_4 = v_startReal_4;

                __m256d v_zImag_1 = v_startImag;
                __m256d v_zImag_2 = v_startImag;
                __m256d v_zImag_3 = v_startImag;
                __m256d v_zImag_4 = v_startImag;

                for (unsigned int counter = 0; counter < max_iters; counter++) {
                    const __m256d v_r2_1 = _mm256_mul_pd(v_zReal_1, v_zReal_1);
                    const __m256d v_r2_2 = _mm256_mul_pd(v_zReal_2, v_zReal_2);
                    const __m256d v_r2_3 = _mm256_mul_pd(v_zReal_3, v_zReal_3);
                    const __m256d v_r2_4 = _mm256_mul_pd(v_zReal_4, v_zReal_4);

                    const __m256d v_i2_1 = _mm256_mul_pd(v_zImag_1, v_zImag_1);
                    const __m256d v_i2_2 = _mm256_mul_pd(v_zImag_2, v_zImag_2);
                    const __m256d v_i2_3 = _mm256_mul_pd(v_zImag_3, v_zImag_3);
                    const __m256d v_i2_4 = _mm256_mul_pd(v_zImag_4, v_zImag_4);

                    v_zImag_1 = _mm256_add_pd(_mm256_mul_pd(v_2_0f, _mm256_mul_pd(v_zReal_1, v_zImag_1)), v_startImag);
                    v_zImag_2 = _mm256_add_pd(_mm256_mul_pd(v_2_0f, _mm256_mul_pd(v_zReal_2, v_zImag_2)), v_startImag);
                    v_zImag_3 = _mm256_add_pd(_mm256_mul_pd(v_2_0f, _mm256_mul_pd(v_zReal_3, v_zImag_3)), v_startImag);
                    v_zImag_4 = _mm256_add_pd(_mm256_mul_pd(v_2_0f, _mm256_mul_pd(v_zReal_4, v_zImag_4)), v_startImag);

                    v_zReal_1 = _mm256_add_pd(_mm256_sub_pd(v_r2_1, v_i2_1), v_startReal_1);
                    v_zReal_2 = _mm256_add_pd(_mm256_sub_pd(v_r2_2, v_i2_2), v_startReal_2);
                    v_zReal_3 = _mm256_add_pd(_mm256_sub_pd(v_r2_3, v_i2_3), v_startReal_3);
                    v_zReal_4 = _mm256_add_pd(_mm256_sub_pd(v_r2_4, v_i2_4), v_startReal_4);

                    const __m256d v_r2_plus_i2_1 = _mm256_add_pd(v_r2_1, v_i2_1);
                    const __m256d v_r2_plus_i2_2 = _mm256_add_pd(v_r2_2, v_i2_2);
                    const __m256d v_r2_plus_i2_3 = _mm256_add_pd(v_r2_3, v_i2_3);
                    const __m256d v_r2_plus_i2_4 = _mm256_add_pd(v_r2_4, v_i2_4);

                    const __m256d v_cmp_res_1 = _mm256_cmp_pd(v_r2_plus_i2_1, v_4_0f, _CMP_LT_OS);
                    const __m256d v_cmp_res_2 = _mm256_cmp_pd(v_r2_plus_i2_2, v_4_0f, _CMP_LT_OS);
                    const __m256d v_cmp_res_3 = _mm256_cmp_pd(v_r2_plus_i2_3, v_4_0f, _CMP_LT_OS);
                    const __m256d v_cmp_res_4 = _mm256_cmp_pd(v_r2_plus_i2_4, v_4_0f, _CMP_LT_OS);

                    v_value_1 = _mm256_blendv_pd(v_value_1, _mm256_add_pd(v_value_1, v_1_0f), v_cmp_res_1);
                    v_value_2 = _mm256_blendv_pd(v_value_2, _mm256_add_pd(v_value_2, v_1_0f), v_cmp_res_2);
                    v_value_3 = _mm256_blendv_pd(v_value_3, _mm256_add_pd(v_value_3, v_1_0f), v_cmp_res_3);
                    v_value_4 = _mm256_blendv_pd(v_value_4, _mm256_add_pd(v_value_4, v_1_0f), v_cmp_res_4);

                    const int32_t res_1 = _mm256_movemask_pd(v_cmp_res_1);
                    const int32_t res_2 = _mm256_movemask_pd(v_cmp_res_2);
                    const int32_t res_3 = _mm256_movemask_pd(v_cmp_res_3);
                    const int32_t res_4 = _mm256_movemask_pd(v_cmp_res_4);

                    if( (res_1 | res_2 | res_3 | res_4) == 0 ) {
                        break;
                    }
                }

                _mm_storeu_ps(ptr_o, _mm256_cvtpd_ps (v_value_1)); ptr_o += (sizeof(__m256d)/sizeof(double));
                _mm_storeu_ps(ptr_o, _mm256_cvtpd_ps (v_value_2)); ptr_o += (sizeof(__m256d)/sizeof(double));
                _mm_storeu_ps(ptr_o, _mm256_cvtpd_ps (v_value_3)); ptr_o += (sizeof(__m256d)/sizeof(double));
                _mm_storeu_ps(ptr_o, _mm256_cvtpd_ps (v_value_4)); ptr_o += (sizeof(__m256d)/sizeof(double));

                v_startReal_1 = _mm256_add_pd(v_startReal_1, _mm256_set1_pd(zoom * DPPP_CONV_STEP));
                v_startReal_2 = _mm256_add_pd(v_startReal_2, _mm256_set1_pd(zoom * DPPP_CONV_STEP));
                v_startReal_3 = _mm256_add_pd(v_startReal_3, _mm256_set1_pd(zoom * DPPP_CONV_STEP));
                v_startReal_4 = _mm256_add_pd(v_startReal_4, _mm256_set1_pd(zoom * DPPP_CONV_STEP));
            }
        }
    }
#endif
}


bool DP_AVX2_u4::is_valid()
{
#ifdef __AVX2__
    return true;
#else
    return false;
#endif
}
