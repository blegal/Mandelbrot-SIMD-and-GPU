#include "DP_SSE4.hpp"

#if __SSE4_1__
    #include "vectclass/vectorclass.h"  // library for INTEL AVX2/AVX512 procesors
#endif

DP_SSE4::DP_SSE4() : Convergence("DP_SSE4")
{
    fractal     = "mandelbrot";
    dataFormat  = "double";
    modeSIMD    = "SSE4";
    modeOPENMP  = "disable";
    OTHER       = "vectclass";
}

DP_SSE4::DP_SSE4(ColorMap *_colors, int _max_iters) : Convergence("DP_SSE4")
{
    colors      = _colors;
    max_iters   = _max_iters;

    fractal     = "mandelbrot";
    dataFormat  = "double";
    modeSIMD    = "SSE4";
    modeOPENMP  = "enable";
    OTHER       = "vectclass";

}

DP_SSE4::~DP_SSE4()
{

}


void DP_SSE4::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
#ifdef __SSE4_1__
    const double f_zoom    = (double) _zoom;
    const double f_offsetX = (double) _offsetX;
    const double f_offsetY = (double) _offsetY;

    const Vec2d cste_1f(1.0f);
    const Vec2d cste_2f(2.0f);
    const Vec2d cste_4f(4.0f);

    const double sReal = f_offsetX - IMAGE_WIDTH  / 2.0 * f_zoom;
    const double sImag = f_offsetY - IMAGE_HEIGHT / 2.0 * f_zoom;

    const Vec2d XStep(2.0 * f_zoom);

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
#endif
}


bool DP_SSE4::is_valid()
{
#ifdef __AVX2__
    return true;
#else
    return false;
#endif
}

