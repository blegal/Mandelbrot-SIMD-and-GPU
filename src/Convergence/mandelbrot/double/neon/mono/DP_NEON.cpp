#include "DP_NEON.hpp"

#ifdef __ARM_NEON__
    #include <arm_neon.h>
#endif

DP_NEON::DP_NEON() : Convergence("DP_NEON")
{
    fractal    = "mandelbrot";
    dataFormat = "double";
    modeSIMD   = "NEON";
    modeOPENMP = "disable";
    OTHER      = "none";
}


DP_NEON::DP_NEON(ColorMap *_colors, int _max_iters) : Convergence("DP_NEON")
{
    colors     = _colors;
    max_iters  = _max_iters;

    fractal    = "mandelbrot";
    dataFormat = "double";
    modeSIMD   = "NEON";
    modeOPENMP = "disable";
    OTHER      = "none";
}


DP_NEON::~DP_NEON()
{

}


#ifdef __ARM_NEON__
inline int _arm_movemask_ps(const uint32x4_t a)
{
    uint32x4_t input = a;
#if defined(__aarch64__)
    static const int32x4_t shift = {0, 1, 2, 3};
    uint32x4_t tmp = vshrq_n_u32(input, 31);
    return vaddvq_u32(vshlq_u32(tmp, shift));
#else
    uint64x2_t high_bits = vreinterpretq_u64_u32(vshrq_n_u32(input, 31));
    uint8x16_t paired = vreinterpretq_u8_u64(vsraq_n_u64(high_bits, high_bits, 31));
    return vgetq_lane_u8(paired, 0) | (vgetq_lane_u8(paired, 8) << 2);
#endif
}
#endif


void DP_NEON::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY,
                          const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float *ptr) {
#ifdef __ARM_NEON__
    const float f_zoom    = (float)_zoom;
    const float f_offsetX = (float)_offsetX;
    const float f_offsetY = (float)_offsetY;

    // consts :
    const float32x4_t v_4_0f = vdupq_n_f32(4.0f);
    const float32x4_t v_2_0f = vdupq_n_f32(2.0f);
    const float32x4_t v_1_0f = vdupq_n_f32(1.0f);

    const float  startR  = f_offsetX - IMAGE_WIDTH  / 2.0f * f_zoom;

    const float /*ALIGN_STRUCT(16)*/ data[4] = {
            startR,
            startR + f_zoom,
            startR + f_zoom * 2.0f,
            startR + f_zoom * 3.0f};

    const int32_t SP_CONV_STEP     = sizeof(float32x4_t)/sizeof(float);
    const float32x4_t startReal    = vld1q_f32(data);
    const float32x4_t XStep        = vdupq_n_f32(f_zoom * SP_CONV_STEP);

    for (uint32_t y = 0; y < IMAGE_HEIGHT; y++)
    {

        const float startImag = f_offsetY - IMAGE_HEIGHT / 2.0f * f_zoom + (y * f_zoom);

        const float32x4_t v_startImag = vdupq_n_f32( startImag );
              float32x4_t v_startReal = startReal;

        float* ptr_o = ptr + y * IMAGE_WIDTH;

        for (uint32_t x = 0; x < IMAGE_WIDTH; x += SP_CONV_STEP)
        {
            float32x4_t v_value = vdupq_n_f32(0);
            float32x4_t v_zReal = v_startReal;
            float32x4_t v_zImag = v_startImag;

            for (uint32_t counter = 0; counter < max_iters; counter++) {
                const float32x4_t v_r2 = vmulq_f32(v_zReal, v_zReal);
                const float32x4_t v_i2 = vmulq_f32(v_zImag, v_zImag);

                v_zImag = vaddq_f32(vmulq_f32(v_2_0f, vmulq_f32(v_zReal, v_zImag)), v_startImag);
                v_zReal = vaddq_f32(vsubq_f32(v_r2, v_i2), v_startReal);

                const float32x4_t v_r2_plus_i2 = vaddq_f32(v_r2, v_i2);
                const uint32x4_t  v_cmp_res    = vcltq_f32(v_r2_plus_i2, v_4_0f);
                const float32x4_t u_value      = vaddq_f32(v_value, v_1_0f);
                v_value = vbslq_f32(v_cmp_res, u_value, v_value); // si r2+i2 < 4 value++

                const uint32_t res = _arm_movemask_ps(v_cmp_res);
                if (res == 0)
                {
                    break;
                }
            }

            vst1q_f32(ptr_o, v_value);
            ptr_o += (sizeof(float32x4_t)/sizeof(float));

            v_startReal = vaddq_f32(v_startReal, XStep);
        }
    }
#endif
}


bool DP_NEON::is_valid() {
#ifdef __ARM_NEON__
    return true;
#else
    return false;
#endif
}
