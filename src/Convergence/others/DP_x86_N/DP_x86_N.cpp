#include "DP_x86_N.hpp"
#include <cmath>

DP_x86_N::DP_x86_N() : Convergence("DP_x86_N", "DP_x86_N")
{
    dataFormat  = "double";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}


DP_x86_N::DP_x86_N(ColorMap* _colors, int _max_iters) : Convergence("DP_x86_N", "DP_x86_N")
{
    colors      = _colors;
    max_iters   = _max_iters;

    dataFormat  = "double";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}


DP_x86_N::~DP_x86_N( ){

}


void DP_x86_N::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
    double offsetX = _offsetX;
    double offsetY = _offsetY;
    double zoom    = _zoom;

    const double n         = 5;
    const double puissance = n / 2.0f;


    for (int y = 0; y < IMAGE_HEIGHT; y++) {

        double startImag = offsetY - IMAGE_HEIGHT / 2.0f * zoom + (y * zoom);
        double startReal = offsetX - IMAGE_WIDTH  / 2.0f * zoom;

        for (int x = 0; x < IMAGE_WIDTH;  x++) {
            int value    = max_iters - 1;
            double zReal = startReal;
            double zImag = startImag;

            for (unsigned int counter = 0; counter < max_iters; counter++) {
                ////////////////////////////////////////////////////////////////////////////////////////////////////////
                double r2 = zReal * zReal;
                double i2 = zImag * zImag;
                double sum1 = r2 + i2;
                double puissance2 = pow(sum1, puissance);
                double zarb = atan2(zImag, zReal);
                double mul1 = n * zarb;
                double sinu = sin(mul1);
                double cosi = cos(mul1);
                zImag = puissance2 * sinu + startImag;
                zReal = puissance2 * cosi + startReal;
                if ( (r2 + i2) > 4.0f) {
                    value = counter;
                    break;
                }
                ////////////////////////////////////////////////////////////////////////////////////////////////////////
            }
            (*ptr++) = value;
            startReal += zoom;
        }
    }
}


bool DP_x86_N::is_valid()
{
    return true;
}

