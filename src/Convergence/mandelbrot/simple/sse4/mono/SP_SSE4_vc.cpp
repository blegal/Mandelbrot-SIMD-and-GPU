#include "SP_SSE4_vc.hpp"

#if defined(__AVX2__)
    #include "vectclass/vectorclass.h"
#endif


SP_SSE4_vc::SP_SSE4_vc() : Convergence("SP_SSE4_vc")
{
    fractal     = "mandelbrot";
    dataFormat  = "float";
    modeSIMD    = "SSE4";
    modeOPENMP  = "enable";
    OTHER       = "vectclass";
}


SP_SSE4_vc::SP_SSE4_vc(ColorMap *_colors, int _max_iters) : Convergence("SP_SSE4_vc")
{
    fractal     = "mandelbrot";
    colors      = _colors;
    max_iters   = _max_iters;

    fractal     = "mandelbrot";
    dataFormat  = "float";
    modeSIMD    = "SSE4";
    modeOPENMP  = "enable";
    OTHER       = "vectclass";
}


SP_SSE4_vc::~SP_SSE4_vc()
{

}


#define SP_CONV_STEP 4


void SP_SSE4_vc::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
#ifdef __SSE4_2__
    const float f_zoom = (float) _zoom;
    const float f_offsetX = (float) _offsetX;
    const float f_offsetY = (float) _offsetY;

    // consts :
    const Vec4f cste_1f(1.0f);
    const Vec4f cste_2f(2.0f);
    const Vec4f cste_4f(4.0f);

    const float startR = f_offsetX - IMAGE_WIDTH / 2.0f * f_zoom;
    const Vec4f startReal(startR, startR + f_zoom * 1.0f, startR + f_zoom * 2.0f, startR + f_zoom * 3.0f);
    const Vec4f XStep(f_zoom * startReal.size());

        for (uint32_t y = 0; y < IMAGE_HEIGHT; y++) {

            float* ptr_o = ptr + y * IMAGE_WIDTH;

            const float startImag = f_offsetY - IMAGE_HEIGHT / 2.0f * f_zoom + (y * f_zoom);
            Vec4f v_startReal = startReal;
            const Vec4f v_startImag(startImag);

            for (uint32_t x = 0; x < IMAGE_WIDTH; x += v_startReal.size()) {

                Vec4f v_value = 0.0f;
                Vec4f v_zReal = v_startReal;
                Vec4f v_zImag = v_startImag;

                for (uint32_t counter = 0; counter < max_iters; counter++) {
                    const Vec4f v_r2 = v_zReal * v_zReal;
                    const Vec4f v_i2 = v_zImag * v_zImag;

                    v_zImag = (cste_2f * (v_zReal * v_zImag)) + v_startImag;
                    v_zReal = (v_r2 - v_i2) + v_startReal;

                    const Vec4f v_r2_plus_i2 = (v_r2 + v_i2);

                    const Vec4fb v_cmp_res = v_r2_plus_i2 < cste_4f; // Tout le monde > 4.0f

                    const Vec4f u_value = v_value + cste_1f;
                    v_value = select(v_cmp_res, u_value, v_value); // si r2+i2 < 4 value++

                    const uint32_t res = horizontal_or(v_cmp_res);
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


bool SP_SSE4_vc::is_valid()
{
#ifdef __SSE4_2__
    return true;
#else
    return false;
#endif
}
