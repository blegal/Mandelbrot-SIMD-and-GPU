#include "QP_x86.hpp"

QP_x86::QP_x86() : Convergence("QP")
{
    dataFormat  = "quad";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}


QP_x86::QP_x86(ColorMap* _colors, int _max_iters) : Convergence("QP")
{
    colors      = _colors;
    max_iters   = _max_iters;

    dataFormat  = "quad";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";

}


QP_x86::~QP_x86( ){

}


void QP_x86::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, sf::Image& image)
{
    const long double offsetX = _offsetX;
    const long double offsetY = _offsetY;
    const long double zoom    = _zoom;

    for (int y = 0; y < IMAGE_HEIGHT; y++) {

        long double startImag = offsetY - IMAGE_HEIGHT / 2.0f * zoom + (y * zoom);
        long double startReal = offsetX - IMAGE_WIDTH  / 2.0f * zoom;

        for (int x = 0; x < IMAGE_WIDTH;  x++) {
            int value    = max_iters - 1;
            long double zReal = startReal;
            long double zImag = startImag;

            for (unsigned int counter = 0; counter < max_iters; counter++) {
                const long double r2 = zReal * zReal;
                const long double i2 = zImag * zImag;
                zImag = 2.0f * zReal * zImag + startImag;
                zReal = r2 - i2 + startReal;
                if ( (r2 + i2) > 4.0f) {
                    value = counter;
                    break;
                }
            }
            image.setPixel(x, y, colors->getColor(value));
            startReal += zoom;
        }
    }
}

void QP_x86::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
    const long double offsetX = _offsetX;
    const long double offsetY = _offsetY;
    const long double zoom    = _zoom;

    for (uint32_t y = 0; y < IMAGE_HEIGHT; y++) {

        long double startImag = offsetY - IMAGE_HEIGHT / 2.0 * zoom + (y * zoom);
        long double startReal = offsetX - IMAGE_WIDTH  / 2.0 * zoom;

        for (uint32_t x = 0; x < IMAGE_WIDTH;  x++) {
            int value    = max_iters - 1;
            long double zReal = startReal;
            long double zImag = startImag;

            for (uint32_t counter = 0; counter < max_iters; counter++) {
                const long double r2 = zReal * zReal;
                const long double i2 = zImag * zImag;
                zImag = 2.0 * zReal * zImag + startImag;
                zReal = r2 - i2 + startReal;
                if ( (r2 + i2) > 4.0) {
                    value = counter;
                    break;
                }
            }
            (*ptr++) = value;
            startReal += zoom;
        }
    }
}


bool QP_x86::is_valid()
{
#ifdef __AVX2__
    return true;
#else
    return false;
#endif
}
