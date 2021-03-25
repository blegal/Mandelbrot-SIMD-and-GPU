#include "JU_SP_AVX512_OMP.hpp"

JU_SP_AVX512_OMP::JU_SP_AVX512_OMP() : Convergence("JU_SP_AVX512_OMP")
{
    fractal     = "julia";
    dataFormat  = "float";
    modeSIMD    = "AVX512";
    modeOPENMP  = "enable";
    OTHER       = "vectclass";
}

JU_SP_AVX512_OMP::JU_SP_AVX512_OMP(ColorMap *_colors, int _max_iters) : Convergence("JU_SP_AVX512_OMP")
{
    colors      = _colors;
    max_iters   = _max_iters;

    fractal     = "julia";
    dataFormat  = "float";
    modeSIMD    = "AVX512";
    modeOPENMP  = "enable";
    OTHER       = "vectclass";
}

JU_SP_AVX512_OMP::~JU_SP_AVX512_OMP()
{

}

void JU_SP_AVX512_OMP::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
#ifdef __AVX2__
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

#pragma omp parallel default (shared)// on declare une section parallel
    {

#pragma omp for // on fait du multicoeur
        for (int y = 0; y < IMAGE_HEIGHT; y++) {

            float* ptr_o = ptr + y * IMAGE_WIDTH;

            const float startImag = f_offsetY - IMAGE_HEIGHT / 2.0f * f_zoom + (y * f_zoom);
            Vec16f v_startReal = startReal;
            Vec16f v_startImag(startImag);

            for (uint32_t x = 0; x < IMAGE_WIDTH; x += v_startReal.size()) {

                Vec16f v_value = 0.0f;
                Vec16f v_zReal = v_startReal;
                Vec16f v_zImag = v_startImag;

                Vec16f v_cReal(0.25f);
                Vec16f v_cImag(0.00f);

                for (uint32_t counter = 0; counter < max_iters; counter++) {
                    const Vec16f v_r2 = v_zReal * v_zReal;
                    const Vec16f v_i2 = v_zImag * v_zImag;

                    v_zImag = cste_2f * (v_zReal * v_zImag) + v_cImag;
                    v_zReal = (v_r2 - v_i2)                 + v_cReal;

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
    }
#endif
}

bool JU_SP_AVX512_OMP::is_valid()
{
#if defined(__AVX2__)
    return true;
#else
    return false;
#endif
}
