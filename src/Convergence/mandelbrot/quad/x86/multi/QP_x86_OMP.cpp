#include "QP_x86_OMP.hpp"


QP_x86_OMP::QP_x86_OMP() : Convergence("QP_OMP"){
    dataFormat  = "quad";
    modeSIMD    = "none";
    modeOPENMP  = "enable";
    OTHER       = "none";
}

QP_x86_OMP::QP_x86_OMP(ColorMap* _colors, int _max_iters) : Convergence("QP_OMP"){
    colors      = _colors;
    max_iters   = _max_iters;

    dataFormat  = "quad";
    modeSIMD    = "none";
    modeOPENMP  = "enable";
    OTHER       = "none";

}

QP_x86_OMP::~QP_x86_OMP( ){

}

uint32_t QP_x86_OMP::process(const long double startReal, const long double startImag, uint32_t max_iters)  {
    long double zReal = startReal;
    long double zImag = startImag;

    for (uint32_t counter = 0; counter < max_iters; counter++) {
        const long double r2 = zReal * zReal;
        const long double i2 = zImag * zImag;
        zImag = 2.0f * zReal * zImag + startImag;
        zReal = r2 - i2 + startReal;
        if ( (r2 + i2) > 4.0f) {
            return counter;
        }
    }
    return (max_iters - 1);
}


void QP_x86_OMP::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, sf::Image& image) {

    const long double offsetX = _offsetX;
    const long double offsetY = _offsetY;
    const long double zoom    = _zoom;

#pragma omp parallel
    {

#pragma omp for
        for (int y = 0; y < IMAGE_HEIGHT; y++) {

            long double imag = offsetY - IMAGE_HEIGHT / 2.0f * zoom + (y * zoom);
            long double real = offsetX - IMAGE_WIDTH / 2.0f * zoom;
            for (int x = 0; x < IMAGE_WIDTH;  x++) {
                int value = process(real, imag, max_iters);
                image.setPixel(x, y, colors->getColor(value));
                real += zoom;
            }
        }
    }
}


void QP_x86_OMP::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr) {

    const long double offsetX = _offsetX;
    const long double offsetY = _offsetY;
    const long double zoom    = _zoom;

#pragma omp parallel // on declare une section parallel
    {

#pragma omp for // on fait du multicoeur
        for (int y = 0; y < IMAGE_HEIGHT; y++) {

            long double imag = offsetY - IMAGE_HEIGHT / 2.0f * zoom + (y * zoom);
            long double real = offsetX - IMAGE_WIDTH / 2.0f * zoom;
            for (int x = 0; x < IMAGE_WIDTH;  x++) {
                int value = process(real, imag, max_iters);
                (*ptr++) = value;
                real += zoom;
            }
        }
    }
}


bool QP_x86_OMP::is_valid()
{
    return true;
}
