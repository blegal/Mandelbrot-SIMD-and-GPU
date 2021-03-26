#include "DP_x86_OMP.hpp"

DP_x86_OMP::DP_x86_OMP() : Convergence("DP_x86_OMP", "Mandelbrot")
{
    dataFormat  = "double";
    modeSIMD    = "none";
    modeOPENMP  = "enable";
    OTHER       = "none";
}

DP_x86_OMP::DP_x86_OMP(ColorMap* _colors, int _max_iters) : Convergence("DP_x86_OMP", "Mandelbrot")
{
    colors      = _colors;
    max_iters   = _max_iters;

    dataFormat  = "double";
    modeSIMD    = "none";
    modeOPENMP  = "enable";
    OTHER       = "none";
}

DP_x86_OMP::~DP_x86_OMP( )
{

}

unsigned int DP_x86_OMP::process(const double startReal, const double startImag, unsigned int max_iters)
{
    double zReal = startReal;
    double zImag = startImag;

    for (unsigned int counter = 0; counter < max_iters; counter++) {
        double r2 = zReal * zReal;
        double i2 = zImag * zImag;
        zImag = 2.0f * zReal * zImag + startImag;
        zReal = r2 - i2 + startReal;
        if ( (r2 + i2) > 4.0f) {
            return counter;
        }
    }
    return max_iters - 1;
}


void DP_x86_OMP::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
    const double offsetX = _offsetX;
    const double offsetY = _offsetY;
    const double zoom    = _zoom;

#pragma omp parallel // on declare une section parallel
    {
#pragma omp for // on fait du multicoeur
        for (int y = 0; y < IMAGE_HEIGHT; y++) {

            float* ptr_o = ptr + y * IMAGE_WIDTH;

            double imag = offsetY - IMAGE_HEIGHT / 2.0f * zoom + (y * zoom);
            double real = offsetX - IMAGE_WIDTH  / 2.0f * zoom;

            for (int x = 0; x < IMAGE_WIDTH;  x++) {
                int value = process(real, imag, max_iters);
                (*ptr_o++) = value;
                real += zoom;
            }
        }
    }
}


bool DP_x86_OMP::is_valid()
{
    return true;
}

