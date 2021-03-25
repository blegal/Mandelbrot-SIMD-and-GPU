#if 0
#if defined(__SSE4_1__)
    #include <immintrin.h>
#endif

#include "../../Convergence.hpp"
#include "../fixed_point.hpp"

#define SSE_CONV_STEP 4

#define V_DEBUG 0
#define DEBUG 0
#define OPEN_MP 1

class Convergence_fp_x86_omp_SSE2 : public Convergence {

public:

    Convergence_fp_x86_omp_SSE2() : Convergence("FP_OMP_SSE2")
    {
        dataFormat  = "fixed";
        modeSIMD    = "SSE4";
        modeOPENMP  = "enable";
        OTHER       = "none";
    }

    Convergence_fp_x86_omp_SSE2(ColorMap* _colors, int _max_iters) : Convergence("FP_OMP_SSE2") {
        colors      = _colors;
        max_iters   = _max_iters;

        dataFormat  = "fixed";
        modeSIMD    = "SSE4";
        modeOPENMP  = "enable";
        OTHER       = "none";
    }

    ~Convergence_fp_x86_omp_SSE2() {
    }

#if 0
    void disp_cmp_real(double float_val, fi32_t fixed_val, unsigned int fractionalBitCount) {
        std::cout << "float = " << float_val << ", fixed = " << fi32_to_double(fixed_val, fractionalBitCount) << std::endl;
    }

    void m128i_show(__m128i v_fixed_val, string varName) {
        __attribute__ ((aligned (32))) int32_t tmp[4] = {0, 0, 0, 0};

        tmp[0] = _mm_extract_epi32(v_fixed_val, 3);
        tmp[1] = _mm_extract_epi32(v_fixed_val, 2);
        tmp[2] = _mm_extract_epi32(v_fixed_val, 1);
        tmp[3] = _mm_extract_epi32(v_fixed_val, 0);

        for (int i = 0 ; i < 4 ; i++) {
            std::bitset<32> y(tmp[i]);
            std::cout << varName;
            for (int j = 0 ; j < 20-varName.length() ; j++) putchar(' ');
            std::cout << "[" << i << "] = 0b" << y;
            printf(" = 0x%0.8x = %11d = %3.10f\n", tmp[i], tmp[i], fi32_to_double(tmp[i], FI_32_25));
        }
        for (int j = 0 ; j <87 ; j++) putchar('-');
        putchar('\n');
    }

    void m128i_show64(__m128i v_fixed_val1, __m128i v_fixed_val2, string varName) {
        __attribute__ ((aligned (32))) int32_t stmp1[4] = {0, 0, 0, 0};
        __attribute__ ((aligned (32))) int32_t stmp2[4] = {0, 0, 0, 0};

        stmp1[0] = _mm_extract_epi32(v_fixed_val1, 3);
        stmp1[1] = _mm_extract_epi32(v_fixed_val1, 2);
        stmp1[2] = _mm_extract_epi32(v_fixed_val1, 1);
        stmp1[3] = _mm_extract_epi32(v_fixed_val1, 0);

        stmp2[0] = _mm_extract_epi32(v_fixed_val2, 3);
        stmp2[1] = _mm_extract_epi32(v_fixed_val2, 2);
        stmp2[2] = _mm_extract_epi32(v_fixed_val2, 1);
        stmp2[3] = _mm_extract_epi32(v_fixed_val2, 0);

        int32_t tmp1[4] = {stmp1[0], stmp1[2], stmp2[0], stmp2[2]};
        int32_t tmp2[4] = {stmp1[1], stmp1[3], stmp2[1], stmp2[3]};
        for (int i = 0 ; i < 4 ; i++) {
            std::bitset<32> y1(tmp1[i]);
            std::bitset<32> y2(tmp2[i]);
            std::cout << varName;
            for (int j = 0 ; j < 20-varName.length() ; j++) putchar(' ');
            std::cout << "[" << i << "] = 0b" << y1 << y2;
            //printf(" = 0x%0.8x%0.8x = %22ld\n", tmp1[i], tmp2[i], (((int64_t)tmp1[i]) << 32) + (int64_t)tmp2[i]);
        }
        for (int j = 0 ; j <87 ; j++) putchar('-');
        putchar('\n');
    }

    void print_fi32_bin(fi32_t fixed_val, string name)  {
        std::bitset<32> y(fixed_val);
        std::cout << name;
        for (int j = 0 ; j < 20-name.length() ; j++) putchar(' ');
        std::cout << "    = 0b" << y;
        printf(" = 0x%0.8x = %11d = %3.10f\n", fixed_val, fixed_val, fi32_to_double(fixed_val, FI_32_25));
        for (int j = 0 ; j < 87 ; j++) putchar('-');
        putchar('\n');
    }

    inline __m128i my_pack_epi64(__m128i a, __m128i b) {
        const __m128i V_MAX_FI_32 = _mm_set1_epi32(MAX_FI_32);

        // échange des deux premiers entre eux et des deux derniers entre eux :
        //__m128i b_shuffled = _mm_shuffle_epi32(b, _MM_SHUFFLE(2, 3, 0, 1));

        // saturation :
        /*__m128i v_cmp0_res;
        v_cmp0_res = _mm_cmpeq_epi32(b, _mm_setzero_si128());
        __m128i b_sat = _mm_blendv_epi8(V_MAX_FI_32, b_shuffled, v_cmp0_res); // MAX si 0x00000000 (!= 0)
        __m128i a_shuffled = _mm_shuffle_epi32(a, _MM_SHUFFLE(2, 3, 0, 1));
        v_cmp0_res = _mm_cmpeq_epi32(a_shuffled, _mm_setzero_si128());
        __m128i a_sat = _mm_blendv_epi8(V_MAX_FI_32, a, v_cmp0_res); // MAX si 0x00000000 (!= 0)*/

        // shuffle :
        __m128i a_sat = _mm_shuffle_epi32(a, _MM_SHUFFLE(2, 0, 2, 0));
        __m128i b_sat = _mm_shuffle_epi32(b, _MM_SHUFFLE(2, 0, 2, 0));

        //a_sat = _mm_shuffle_epi32(a_sat, _MM_SHUFFLE(2, 0, 2, 0));
        //b_sat = _mm_shuffle_epi32(b_sat, _MM_SHUFFLE(3, 1, 3, 1));

        // pack :
        __m128i blendmask = _mm_set_epi32(0x00000000, 0x00000000, 0xffffffff, 0xffffffff);
        __m128i result = _mm_blendv_epi8(a_sat, b_sat, blendmask); // b si 0xffffffff, a si 0x00000000
        return result;
    }
#endif

    virtual void updateImage(const long double d_zoom, const long double d_offsetX, const long double d_offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, sf::Image& image) {
#if defined(__SSE4_1__)
        const __m128i V_4_SHIFTED = _mm_set1_epi32(4 << FI_32_25);
        const __m128i V_4 = _mm_set1_epi32(4);
        const __m128i V_2 = _mm_set1_epi32(2);
        const __m128i V_1 = _mm_set1_epi32(1);
        const __m128i V_1_64 = _mm_set1_epi64x(1);
        const __m128i V_0 = _mm_setzero_si128();
        const __m128i V_FI_32_25 = _mm_set_epi32(0, FI_32_25, 0, FI_32_25);

#if OPEN_MP
#pragma omp parallel // on declare une section parallel
        {
#pragma omp for // on fait du multicoeur
#endif
            for (int y = 0; y < IMAGE_HEIGHT; y++) {
                fi32_t offsetX = double_to_fi32(d_offsetX, FI_32_25);
                fi32_t offsetY = double_to_fi32(d_offsetY, FI_32_25);

                fi32_t zoom = double_to_fi32(d_zoom, FI_32_25);

                fi32_t startImag = offsetY  -  IMAGE_HEIGHT/2 * zoom  +  y * zoom;
                __m128i v_startImag = _mm_set1_epi32(startImag); //startImag dans tous les elements

                fi32_t startReal = offsetX  -  IMAGE_WIDTH/2 * zoom;
                __m128i v_startReal = _mm_set_epi32(startReal, startReal+zoom, startReal+zoom*2, startReal+zoom*3);

                for (int x = 0; x < IMAGE_WIDTH;  x+= SSE_CONV_STEP) {
//                    int value = max_iters - 1;
                    __m128i v_value = V_0;

                    fi32_t zReal = startReal;
                    __m128i v_zReal = v_startReal; //_mm_set_epi32(0x7fffffff, 0xf0000000, 80, 18);
                    fi32_t zImag = startImag;
                    __m128i v_zImag = v_startImag;
#if DEBUG
                    printf("-------------------------\n\tx = %d\n -------------------------\n", x);
#endif

                    for (unsigned int counter = 0; counter < max_iters; counter++) {
#if DEBUG
                        printf("-------------------------\n\titer = %d\n -------------------------\n", counter);
#endif
                        fi64_t r2_64 = ((fi64_t)(zReal)*(fi64_t)(zReal));
                        fi32_t r2 = ((r2_64 >> FI_32_25) < MAX_FI_32) ? (fi32_t)(r2_64 >> FI_32_25) : MAX_FI_32;

                        __m128i v_zReal_64_1 = _mm_unpackhi_epi32(v_zReal, _mm_cmplt_epi32(v_zReal, V_0)); // passage sur 64 bits avec extension de signe
                        __m128i v_zReal_64_2 = _mm_unpacklo_epi32(v_zReal, _mm_cmplt_epi32(v_zReal, V_0));
                        __m128i v_r2_64_1 = _mm_mul_epi32(v_zReal_64_1, v_zReal_64_1);
                        __m128i v_r2_64_2 = _mm_mul_epi32(v_zReal_64_2, v_zReal_64_2);
                        v_r2_64_1 = _mm_srl_epi64(v_r2_64_1, V_FI_32_25);
                        v_r2_64_2 = _mm_srl_epi64(v_r2_64_2, V_FI_32_25);

                        __m128i v_r2 = my_pack_epi64(v_r2_64_1, v_r2_64_2);
#if DEBUG
                        print_fi32_bin(r2, "r2");
            m128i_show(v_r2, "v_r2");
#endif
                        fi64_t i2_64 = ((fi64_t)(zImag)*(fi64_t)(zImag));
                        fi32_t i2 = ((i2_64 >> FI_32_25) < MAX_FI_32) ? (fi32_t)(i2_64 >> FI_32_25) : MAX_FI_32;

                        __m128i v_zImag_64_1 = _mm_unpackhi_epi32(v_zImag, _mm_cmplt_epi32(v_zImag, V_0)); // passage sur 64 bits avec extension de signe
                        __m128i v_zImag_64_2 = _mm_unpacklo_epi32(v_zImag, _mm_cmplt_epi32(v_zImag, V_0));
                        __m128i v_i2_64_1 = _mm_mul_epi32(v_zImag_64_1, v_zImag_64_1);
                        __m128i v_i2_64_2 = _mm_mul_epi32(v_zImag_64_2, v_zImag_64_2);
                        v_i2_64_1 = _mm_srl_epi64(v_i2_64_1, V_FI_32_25);
                        v_i2_64_2 = _mm_srl_epi64(v_i2_64_2, V_FI_32_25);
                        __m128i v_i2 = my_pack_epi64(v_i2_64_1, v_i2_64_2);
#if DEBUG
                        print_fi32_bin(i2, "i2");
            m128i_show(v_i2, "v_i2");
#endif
                        fi64_t z_real_z_imag_64 = (fi64_t)zReal*(fi64_t)zImag;
                        fi32_t z_real_z_imag = (fi32_t)(z_real_z_imag_64 >> FI_32_25);

                        __m128i v_z_real_z_imag_64_1 = _mm_mul_epi32(v_zReal_64_1, v_zImag_64_1);
                        __m128i v_z_real_z_imag_64_2 = _mm_mul_epi32(v_zReal_64_2, v_zImag_64_2);
                        v_z_real_z_imag_64_1 = _mm_srl_epi64(v_z_real_z_imag_64_1, V_FI_32_25);
                        v_z_real_z_imag_64_2 = _mm_srl_epi64(v_z_real_z_imag_64_2, V_FI_32_25);
                        __m128i v_z_real_z_imag = my_pack_epi64(v_z_real_z_imag_64_1, v_z_real_z_imag_64_2);

#if DEBUG
                        print_fi32_bin(z_real_z_imag, "z_real_z_imag = ");
            m128i_show(v_z_real_z_imag, "v_z_real_z_imag");
#endif

                        zImag = (2 * z_real_z_imag  + startImag);
                        __m128i v_z_real_z_imag_x2 = _mm_sll_epi32(v_z_real_z_imag, V_1_64);

                        v_zImag = _mm_add_epi32(v_z_real_z_imag_x2, v_startImag);
#if DEBUG
                        print_fi32_bin(zImag, "zImag = ");
            m128i_show(v_zImag, "v_zImag");
#endif

                        zReal = (r2  -  i2  +  startReal);
                        v_zReal = _mm_add_epi32(_mm_sub_epi32(v_r2, v_i2), v_startReal);

#if DEBUG
                        print_fi32_bin(zReal, "zReal = ");
            m128i_show(v_zReal, "v_zReal");

            print_fi32_bin(r2, "r2 = ");
            m128i_show(v_r2, "v_r2");
            print_fi32_bin(i2, "i2 = ");
            m128i_show(v_i2, "v_i2");
#endif

                        __m128i v_r2_plus_i2 = _mm_add_epi32(v_r2, v_i2);
#if DEBUG
                        print_fi32_bin(r2+i2, "r2+i2 = ");
            m128i_show(v_r2_plus_i2, "v_r2_plus_i2");
#endif

                        __m128i v_cmp_res = _mm_cmplt_epi32(v_r2_plus_i2, V_4_SHIFTED);
                        v_value = _mm_blendv_epi8(v_value, _mm_add_epi32(v_value, V_1), v_cmp_res); // si r2+i2 < 4 value++
#if V_DEBUG
                        m128i_show(v_value, "v_value");
            //usleep(1000000);
            usleep(10000);
#endif
                        short res = _mm_movemask_epi8(v_cmp_res);
                        if (res == 0) { // si r2+i2 > 4 pour tous les élements
                            break;
                        }
                    }
                    image.setPixel(x+0, y, colors->getColor((int)round(_mm_extract_epi32(v_value, 3))));
                    image.setPixel(x+1, y, colors->getColor((int)round(_mm_extract_epi32(v_value, 2))));
                    image.setPixel(x+2, y, colors->getColor((int)round(_mm_extract_epi32(v_value, 1))));
                    image.setPixel(x+3, y, colors->getColor((int)round(_mm_extract_epi32(v_value, 0))));

                    startReal = startReal + SSE_CONV_STEP*zoom;
                    v_startReal = _mm_add_epi32(v_startReal, _mm_set1_epi32(zoom*SSE_CONV_STEP));
                }
            }
#if OPEN_MP
        }
#endif
#endif
    }

    virtual void updateImage(const long double d_zoom, const long double d_offsetX, const long double d_offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
    {
#if defined(__AVX2__)

        const __m128i V_4_SHIFTED = _mm_set1_epi32(4 << FI_32_25);
        const __m128i V_4 = _mm_set1_epi32(4);
        const __m128i V_2 = _mm_set1_epi32(2);
        const __m128i V_1 = _mm_set1_epi32(1);
        const __m128i V_1_64 = _mm_set1_epi64x(1);
        const __m128i V_0 = _mm_setzero_si128();
        const __m128i V_FI_32_25 = _mm_set_epi32(0, FI_32_25, 0, FI_32_25);

#if OPEN_MP
#pragma omp parallel // on declare une section parallel
        {
#pragma omp for // on fait du multicoeur
#endif
            for (int y = 0; y < IMAGE_HEIGHT; y++) {
                fi32_t offsetX = double_to_fi32(d_offsetX, FI_32_25);
                fi32_t offsetY = double_to_fi32(d_offsetY, FI_32_25);

                fi32_t zoom = double_to_fi32(d_zoom, FI_32_25);

                fi32_t startImag = offsetY  -  IMAGE_HEIGHT/2 * zoom  +  y * zoom;
                __m128i v_startImag = _mm_set1_epi32(startImag); //startImag dans tous les elements

                fi32_t startReal = offsetX  -  IMAGE_WIDTH/2 * zoom;
                __m128i v_startReal = _mm_set_epi32(startReal, startReal+zoom, startReal+zoom*2, startReal+zoom*3);

                float* ptr_o = ptr + y * IMAGE_WIDTH;

                for (int x = 0; x < IMAGE_WIDTH;  x+= SSE_CONV_STEP) {
                    int value = max_iters - 1;
                    __m128i v_value = V_0;

                    fi32_t zReal = startReal;
                    __m128i v_zReal = v_startReal; //_mm_set_epi32(0x7fffffff, 0xf0000000, 80, 18);
                    fi32_t zImag = startImag;
                    __m128i v_zImag = v_startImag;
#if DEBUG
                    printf("-------------------------\n\tx = %d\n -------------------------\n", x);
#endif

                    for (unsigned int counter = 0; counter < max_iters; counter++) {
#if DEBUG
                        printf("-------------------------\n\titer = %d\n -------------------------\n", counter);
#endif
                        fi64_t r2_64 = ((fi64_t)(zReal)*(fi64_t)(zReal));
                        fi32_t r2 = ((r2_64 >> FI_32_25) < MAX_FI_32) ? (fi32_t)(r2_64 >> FI_32_25) : MAX_FI_32;

                        __m128i v_zReal_64_1 = _mm_unpackhi_epi32(v_zReal, _mm_cmplt_epi32(v_zReal, V_0)); // passage sur 64 bits avec extension de signe
                        __m128i v_zReal_64_2 = _mm_unpacklo_epi32(v_zReal, _mm_cmplt_epi32(v_zReal, V_0));
                        __m128i v_r2_64_1 = _mm_mul_epi32(v_zReal_64_1, v_zReal_64_1);
                        __m128i v_r2_64_2 = _mm_mul_epi32(v_zReal_64_2, v_zReal_64_2);
                        v_r2_64_1 = _mm_srl_epi64(v_r2_64_1, V_FI_32_25);
                        v_r2_64_2 = _mm_srl_epi64(v_r2_64_2, V_FI_32_25);

                        __m128i v_r2 = my_pack_epi64(v_r2_64_1, v_r2_64_2);
#if DEBUG
                        print_fi32_bin(r2, "r2");
            m128i_show(v_r2, "v_r2");
#endif
                        fi64_t i2_64 = ((fi64_t)(zImag)*(fi64_t)(zImag));
                        fi32_t i2 = ((i2_64 >> FI_32_25) < MAX_FI_32) ? (fi32_t)(i2_64 >> FI_32_25) : MAX_FI_32;

                        __m128i v_zImag_64_1 = _mm_unpackhi_epi32(v_zImag, _mm_cmplt_epi32(v_zImag, V_0)); // passage sur 64 bits avec extension de signe
                        __m128i v_zImag_64_2 = _mm_unpacklo_epi32(v_zImag, _mm_cmplt_epi32(v_zImag, V_0));
                        __m128i v_i2_64_1 = _mm_mul_epi32(v_zImag_64_1, v_zImag_64_1);
                        __m128i v_i2_64_2 = _mm_mul_epi32(v_zImag_64_2, v_zImag_64_2);
                        v_i2_64_1 = _mm_srl_epi64(v_i2_64_1, V_FI_32_25);
                        v_i2_64_2 = _mm_srl_epi64(v_i2_64_2, V_FI_32_25);
                        __m128i v_i2 = my_pack_epi64(v_i2_64_1, v_i2_64_2);
#if DEBUG
                        print_fi32_bin(i2, "i2");
            m128i_show(v_i2, "v_i2");
#endif
                        fi64_t z_real_z_imag_64 = (fi64_t)zReal*(fi64_t)zImag;
                        fi32_t z_real_z_imag = (fi32_t)(z_real_z_imag_64 >> FI_32_25);

                        __m128i v_z_real_z_imag_64_1 = _mm_mul_epi32(v_zReal_64_1, v_zImag_64_1);
                        __m128i v_z_real_z_imag_64_2 = _mm_mul_epi32(v_zReal_64_2, v_zImag_64_2);
                        v_z_real_z_imag_64_1 = _mm_srl_epi64(v_z_real_z_imag_64_1, V_FI_32_25);
                        v_z_real_z_imag_64_2 = _mm_srl_epi64(v_z_real_z_imag_64_2, V_FI_32_25);
                        __m128i v_z_real_z_imag = my_pack_epi64(v_z_real_z_imag_64_1, v_z_real_z_imag_64_2);

#if DEBUG
                        print_fi32_bin(z_real_z_imag, "z_real_z_imag = ");
            m128i_show(v_z_real_z_imag, "v_z_real_z_imag");
#endif

                        zImag = (2 * z_real_z_imag  + startImag);
                        __m128i v_z_real_z_imag_x2 = _mm_sll_epi32(v_z_real_z_imag, V_1_64);

                        v_zImag = _mm_add_epi32(v_z_real_z_imag_x2, v_startImag);
#if DEBUG
                        print_fi32_bin(zImag, "zImag = ");
            m128i_show(v_zImag, "v_zImag");
#endif

                        zReal = (r2  -  i2  +  startReal);
                        v_zReal = _mm_add_epi32(_mm_sub_epi32(v_r2, v_i2), v_startReal);

#if DEBUG
                        print_fi32_bin(zReal, "zReal = ");
            m128i_show(v_zReal, "v_zReal");

            print_fi32_bin(r2, "r2 = ");
            m128i_show(v_r2, "v_r2");
            print_fi32_bin(i2, "i2 = ");
            m128i_show(v_i2, "v_i2");
#endif

                        __m128i v_r2_plus_i2 = _mm_add_epi32(v_r2, v_i2);
#if DEBUG
                        print_fi32_bin(r2+i2, "r2+i2 = ");
            m128i_show(v_r2_plus_i2, "v_r2_plus_i2");
#endif

                        __m128i v_cmp_res = _mm_cmplt_epi32(v_r2_plus_i2, V_4_SHIFTED);
                        v_value = _mm_blendv_epi8(v_value, _mm_add_epi32(v_value, V_1), v_cmp_res); // si r2+i2 < 4 value++
#if V_DEBUG
                        m128i_show(v_value, "v_value");
            //usleep(1000000);
            usleep(10000);
#endif
                        short res = _mm_movemask_epi8(v_cmp_res);
                        if (res == 0) { // si r2+i2 > 4 pour tous les élements
                            break;
                        }
                    }

                    (*ptr_o++) = _mm_extract_epi32(v_value, 3);
                    (*ptr_o++) = _mm_extract_epi32(v_value, 2);
                    (*ptr_o++) = _mm_extract_epi32(v_value, 1);
                    (*ptr_o++) = _mm_extract_epi32(v_value, 0);

                    startReal = startReal + SSE_CONV_STEP*zoom;
                    v_startReal = _mm_add_epi32(v_startReal, _mm_set1_epi32(zoom*SSE_CONV_STEP));
                }
            }
#if OPEN_MP
        }
#endif
#endif
    }

    bool is_valid()
    {
#ifdef __AVX2__
        return true;
#else
        return false;
#endif
    }

};
#endif
