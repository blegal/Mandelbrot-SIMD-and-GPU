#include "DP_AVX2.hpp"

#ifdef __AVX2__
    #include <immintrin.h>
#endif

DP_AVX2::DP_AVX2() : Convergence("DP_AVX2")
{
    dataFormat  = "double";
    modeSIMD    = "AVX2";
    modeOPENMP  = "disable";
    OTHER       = "none";
}


DP_AVX2::DP_AVX2(ColorMap* _colors, int _max_iters) : Convergence("DP_AVX2")
{
    colors      = _colors;
    max_iters   = _max_iters;

    dataFormat  = "double";
    modeSIMD    = "AVX2";
    modeOPENMP  = "disable";
    OTHER       = "none";

}


DP_AVX2::~DP_AVX2()
{

}


void DP_AVX2::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
#ifdef __AVX2__
    const double zoom    = _zoom;
    const double offsetX = _offsetX;
    const double offsetY = _offsetY;

    const __m256d v_4_0f = _mm256_set1_pd(4.0f);
    const __m256d v_2_0f = _mm256_set1_pd(2.0f);
    const __m256d v_1_0f = _mm256_set1_pd(1.0f);
    const __m256d XStep  = _mm256_set1_pd(zoom * CONV_STEP);

    const double sReal = offsetX - IMAGE_WIDTH  / 2.0 * zoom;
    const double sImag = offsetY - IMAGE_HEIGHT / 2.0 * zoom;


        for (int y = 0 ; y < IMAGE_HEIGHT ; y++) {

            float* ptr_o = ptr + y * IMAGE_WIDTH;

                  __m256d v_sReal = _mm256_setr_pd(sReal, sReal + zoom, sReal + 2.0 * zoom, sReal + 3.0 * zoom);
            const __m256d v_sImag = _mm256_set1_pd(sImag + (y * zoom));

            for (int x = 0 ; x < IMAGE_WIDTH ; x += CONV_STEP) {

                __m256d v_value = _mm256_setzero_pd();
                __m256d v_zReal = v_sReal;
                __m256d v_zImag = v_sImag;

                for (unsigned int counter = 0; counter < max_iters; counter++) {
                    const __m256d v_r2 = _mm256_mul_pd(v_zReal, v_zReal);
                    const __m256d v_i2 = _mm256_mul_pd(v_zImag, v_zImag);

                    v_zImag = _mm256_add_pd(_mm256_mul_pd(v_2_0f, _mm256_mul_pd(v_zReal, v_zImag)), v_sImag);
                    v_zReal = _mm256_add_pd(_mm256_sub_pd(v_r2, v_i2), v_sReal);

                    const __m256d v_r2_plus_i2 = _mm256_add_pd(v_r2, v_i2);

                    const __m256d v_cmp_res = _mm256_cmp_pd(v_r2_plus_i2, v_4_0f, _CMP_LT_OS);
                    const __m256d u_value   = _mm256_add_pd(v_value, v_1_0f);
                    v_value                 = _mm256_blendv_pd(v_value, u_value, v_cmp_res); // si r2+i2 < 4 value++

                    const int32_t res = _mm256_movemask_pd(v_cmp_res);
                    if (res == 0) { // si r2+i2 > 4 pour tous les élements
                        break;
                    }
                }

                _mm_storeu_ps(ptr_o, _mm256_cvtpd_ps (v_value));
                ptr_o  += (sizeof(__m128)/sizeof(float));
                v_sReal = _mm256_add_pd(v_sReal, XStep);
            }
        }
#endif
}


bool DP_AVX2::is_valid()
{
#ifdef __AVX2__
    return true;
#else
    return false;
#endif
}
