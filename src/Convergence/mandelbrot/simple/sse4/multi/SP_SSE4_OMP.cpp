#include "SP_SSE4_OMP.hpp"

#ifdef __SSE4_2__
#include <immintrin.h>
#endif

#define SP_CONV_STEP 4

SP_SSE4_OMP::SP_SSE4_OMP() : Convergence("SP_SSE4_OMP")
{
    fractal     = "mandelbrot";
    dataFormat  = "float";
    modeSIMD    = "SSE4";
    modeOPENMP  = "enable";
    OTHER       = "none";
}


SP_SSE4_OMP::SP_SSE4_OMP(ColorMap *_colors, int _max_iters) : Convergence("SP_SSE4_OMP")
{
    colors      = _colors;
    max_iters   = _max_iters;

    fractal     = "mandelbrot";
    dataFormat  = "float";
    modeSIMD    = "SSE4";
    modeOPENMP  = "enable";
    OTHER       = "none";
}


SP_SSE4_OMP::~SP_SSE4_OMP()
{

}


void SP_SSE4_OMP::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
#ifdef __SSE4_2__
    const float f_zoom    = (float)_zoom;
    const float f_offsetX = (float)_offsetX;
    const float f_offsetY = (float)_offsetY;

    // consts :
    const __m128 v_4_0f = _mm_set1_ps(4.0f);
    const __m128 v_2_0f = _mm_set1_ps(2.0f);
    const __m128 v_1_0f = _mm_set1_ps(1.0f);

    const float  startR  = f_offsetX - IMAGE_WIDTH  / 2.0f * f_zoom;
    const __m128 startReal = _mm_setr_ps(startR, startR + f_zoom * 1.0f, startR + f_zoom * 2.0f, startR + f_zoom * 3.0f);
    const __m128 XStep     = _mm_set1_ps(f_zoom * SP_CONV_STEP);

#pragma omp parallel
    {
#pragma omp for // on fait du multicoeur
        for (uint32_t y = 0; y < IMAGE_HEIGHT; y++) {

            const float startImag = f_offsetY - IMAGE_HEIGHT / 2.0f * f_zoom + (y * f_zoom);

            __m128 v_startImag = _mm_set1_ps( startImag );
            __m128 v_startReal = startReal;

            float* ptr_o = ptr + y * IMAGE_WIDTH;

            for (uint32_t x = 0; x < IMAGE_WIDTH; x += SP_CONV_STEP) {
                __m128 v_value = _mm_setzero_ps();
                __m128 v_zReal = v_startReal;
                __m128 v_zImag = v_startImag;

                for (uint32_t counter = 0; counter < max_iters; counter++) {
                    __m128 v_r2 = _mm_mul_ps(v_zReal, v_zReal);
                    __m128 v_i2 = _mm_mul_ps(v_zImag, v_zImag);

                    v_zImag = _mm_add_ps(_mm_mul_ps(v_2_0f, _mm_mul_ps(v_zReal, v_zImag)), v_startImag);
                    v_zReal = _mm_add_ps(_mm_sub_ps(v_r2, v_i2), v_startReal);

                    __m128 v_r2_plus_i2 = _mm_add_ps(v_r2, v_i2);
                    __m128 v_cmp_res = _mm_cmp_ps(v_r2_plus_i2, v_4_0f, _CMP_LT_OS);
                    const __m128 u_value = _mm_add_ps(v_value, v_1_0f);
                    v_value = _mm_blendv_ps(v_value, u_value, v_cmp_res); // si r2+i2 < 4 value++

                    const uint32_t res = _mm_movemask_ps(v_cmp_res);
                    if (res == 0) { // si r2+i2 > 4 pour tous les élements
                        break;
                    }
                }

                _mm_storeu_ps(ptr_o, v_value);
                ptr_o += (sizeof(__m128)/sizeof(float));

                v_startReal = _mm_add_ps(v_startReal, XStep);
            }
        }
    }
#endif
}


bool SP_SSE4_OMP::is_valid()
{
#ifdef __SSE4_2__
    return true;
#else
    return false;
#endif
}
