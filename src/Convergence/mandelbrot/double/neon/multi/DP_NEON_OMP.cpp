#include "DP_NEON_OMP.hpp"

#ifdef __ARM_NEON__
    #include <arm_neon.h>
#endif

DP_NEON_OMP::DP_NEON_OMP() : Convergence("DP_NEON_OMP", "mandelbrot")
{
    dataFormat  = "double";
    modeSIMD    = "NEON";
    modeOPENMP  = "enable";
    OTHER       = "none";
}


DP_NEON_OMP::DP_NEON_OMP(ColorMap *_colors, int _max_iters) : Convergence("DP_NEON_OMP", "mandelbrot")
{
    colors      = _colors;
    max_iters   = _max_iters;

    dataFormat  = "double";
    modeSIMD    = "NEON";
    modeOPENMP  = "enable";
    OTHER       = "none";
}


DP_NEON_OMP::~DP_NEON_OMP()
{

}


#ifdef __ARM_NEON__
inline int _arm_movemask_pd(const uint64x2_t a)
{
    uint32x4_t input = a;
    static const int32x4_t shift = {0, 1, 2, 3};
    uint32x4_t tmp = vshrq_n_u32(input, 31);
    return vaddvq_u32(vshlq_u32(tmp, shift));
}
#endif


void DP_NEON_OMP::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
#ifdef __ARM_NEON__
    const double f_zoom    = (double)_zoom;
    const double f_offsetX = (double)_offsetX;
    const double f_offsetY = (double)_offsetY;

    // consts :
    const float64x2_t v_4_0f = vdupq_n_f64(4.0f);
    const float64x2_t v_2_0f = vdupq_n_f64(2.0f);
    const float64x2_t v_1_0f = vdupq_n_f64(1.0f);

    const double startR  = f_offsetX - IMAGE_WIDTH  / 2.0 * f_zoom;

    const double data[2] = {startR, startR + f_zoom};

    const int32_t SP_CONV_STEP     = sizeof(float64x2_t)/sizeof(double);
    const float64x2_t startReal    = vld1q_f64(data);
    const float64x2_t XStep        = vdupq_n_f64(f_zoom * SP_CONV_STEP);

#pragma omp parallel
    {
#pragma omp for // on fait du multicoeur
        for (uint32_t y = 0; y < IMAGE_HEIGHT; y++) {

            const double startImag = f_offsetY - IMAGE_HEIGHT / 2.0f * f_zoom + (y * f_zoom);

            const float64x2_t v_startImag = vdupq_n_f64( startImag );
                  float64x2_t v_startReal = startReal;

            float* ptr_o = ptr + y * IMAGE_WIDTH;

            for (uint32_t x = 0; x < IMAGE_WIDTH; x += SP_CONV_STEP) {
                float64x2_t v_value = vdupq_n_f64(0);
                float64x2_t v_zReal = v_startReal;
                float64x2_t v_zImag = v_startImag;

                for (uint32_t counter = 0; counter < max_iters; counter++) {
                    const float64x2_t v_r2 = vmulq_f64(v_zReal, v_zReal);
                    const float64x2_t v_i2 = vmulq_f64(v_zImag, v_zImag);

                    v_zImag = vaddq_f64(vmulq_f64(v_2_0f, vmulq_f64(v_zReal, v_zImag)), v_startImag);
                    v_zReal = vaddq_f64(vsubq_f64(v_r2, v_i2), v_startReal);

                    const float64x2_t v_r2_plus_i2  = vaddq_f64(v_r2, v_i2);
                    const uint32x4_t  v_cmp_res     = vcltq_f64(v_r2_plus_i2, v_4_0f);
                    const float64x2_t u_value = vaddq_f64(v_value, v_1_0f);
                    v_value = vbslq_f64(v_cmp_res, u_value, v_value); // si r2+i2 < 4 value++

                    const uint32_t res = _arm_movemask_pd(v_cmp_res);
                    if (res == 0)
                    {
                        break;
                    }
                }

                double buffer[2];
                vst1q_f64(buffer, v_value);                      // TODO BUG !!!
                (*ptr_o++) = (float)buffer[0];
                (*ptr_o++) = (float)buffer[1];
//                ptr_o += (sizeof(float64x2_t)/sizeof(float));   // TODO BUG !!!
                v_startReal = vaddq_f64(v_startReal, XStep);
            }
        }
    }
#endif
}


bool DP_NEON_OMP::is_valid()
{
#ifdef __ARM_NEON__
    return true;
#else
    return false;
#endif
}
