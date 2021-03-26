#include "FP_Q32_27_x86.hpp"

FP_Q32_27_x86::FP_Q32_27_x86() : Convergence("FP_Q32_27_x86")
{
    dataFormat  = "fixed";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}

FP_Q32_27_x86::FP_Q32_27_x86(ColorMap* _colors, int _max_iters) : Convergence("FP_Q32_27_x86")
{
    colors      = _colors;
    max_iters   = _max_iters;

    dataFormat  = "fixed";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}

FP_Q32_27_x86::~FP_Q32_27_x86()
{

}

#define Q_FORMAT FI_32_27

void FP_Q32_27_x86::updateImage(const long double d_zoom, const long double d_offsetX, const long double d_offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
#if 0
    const fi32_t offsetX = double_to_fi32(d_offsetX, Q_FORMAT);
    const fi32_t offsetY = double_to_fi32(d_offsetY, Q_FORMAT);
    const fi32_t zoom    = double_to_fi32(d_zoom,    Q_FORMAT);

    fi32_t coin_x = offsetY  -  (IMAGE_HEIGHT/2) * zoom;
    fi32_t coin_y = offsetX  -  (IMAGE_WIDTH/2 ) * zoom;
    printf("(DD) Position de la fenetre (FP_Q32_27_x86)\n");
    printf("(DD) - CENTRE : X = %12d | Y = %12d | S = %12d\n", offsetX, offsetY, zoom);
    printf("(DD) - COIN   : X = %12d | Y = %12d | S = %12d\n", coin_x,   coin_y, zoom);
#endif

    for (int y = 0; y < IMAGE_HEIGHT; y++)
    {
        fi32_t offsetX = double_to_fi32(d_offsetX, Q_FORMAT);
        fi32_t offsetY = double_to_fi32(d_offsetY, Q_FORMAT);
        fi32_t zoom    = double_to_fi32(d_zoom,    Q_FORMAT);

        fi32_t startImag = offsetY  -  IMAGE_HEIGHT/2 * zoom  +  y * zoom;
        fi32_t startReal = offsetX  -  IMAGE_WIDTH/2  * zoom;

        for (int x = 0; x < IMAGE_WIDTH;  x++) {

            int value = max_iters - 1;

            fi32_t zReal = startReal;
            fi32_t zImag = startImag;

            for (unsigned int counter = 0; counter < max_iters; counter++)
            {
                //double d_r2 = d_zReal * d_zReal;
                const fi64_t _r2_64 = ((fi64_t)(zReal)) * ((fi64_t)(zReal));
                const fi64_t _r2_    = _r2_64 >> Q_FORMAT;
                const fi32_t _r2     = (_r2_ < MAX_FI_32) ? _r2_ : MAX_FI_32;  // USELESS ?

                //double d_i2 = d_zImag * d_zImag;
                const fi64_t _i2_64 = (((fi64_t)(zImag)) * ((fi64_t)(zImag)));
                const fi64_t _i2_   = _i2_64 >> Q_FORMAT;
                const fi32_t _i2    = (_i2_ < MAX_FI_32) ? _i2_ : MAX_FI_32;   // USELESS ?

                //double d_i2 = d_zReal * d_zImag;
                const fi64_t _ri_64 = (((fi64_t)(zReal)) * ((fi64_t)(zImag)));
                const fi64_t _ri_   = _ri_64 >> Q_FORMAT;
                const fi32_t _ri    = (_ri_ < MAX_FI_32) ? _ri_ : MAX_FI_32;   // USELESS ?
                const fi32_t _2ri   = 2 * _ri;

                zImag = (_2ri      + startImag);
                zReal = (_r2 - _i2 + startReal);

                if ( (_r2 + _i2) > (4 << Q_FORMAT))
                {
                    value = counter;
                    break;
                }
            }
            (*ptr++) = value;
            startReal = startReal + zoom;
        }
    }
}


bool FP_Q32_27_x86::is_valid()
{
    return true;
}
