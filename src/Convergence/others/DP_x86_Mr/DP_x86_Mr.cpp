#include "DP_x86_Mr.hpp"

//
// Monsieur-brot
//

DP_x86_Mr::DP_x86_Mr() : Convergence("DP_x86_Mr", "Mister Brot")
{
    dataFormat  = "double";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}


DP_x86_Mr::DP_x86_Mr(ColorMap* _colors, int _max_iters) : Convergence("DP_x86_Mr", "Mister Brot")
{
    colors      = _colors;
    max_iters   = _max_iters;

    dataFormat  = "double";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}


DP_x86_Mr::~DP_x86_Mr()
{

}


void DP_x86_Mr::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
    double offsetX = _offsetX;
    double offsetY = _offsetY;
    double zoom    = _zoom;

    for (int y = 0; y < IMAGE_HEIGHT; y++) {

        double startImag = offsetY - IMAGE_HEIGHT / 2.0f * zoom + (y * zoom);
        double startReal = offsetX - IMAGE_WIDTH  / 2.0f * zoom;

        for (int x = 0; x < IMAGE_WIDTH;  x++) {
            int value    = max_iters - 1;
            double zReal = startReal;
            double zImag = startImag;

            for (unsigned int counter = 0; counter < max_iters; counter++) {
                ////////////////////////////////////////////////////////////////////////////////////////////////////////
                zReal = abs(zReal);
                zImag = -zImag;

                double r5 = zReal * zReal * zReal * zReal * zReal;
                double r4 = zReal * zReal * zReal * zReal;
                double r3 = zReal * zReal * zReal;
                double r2 = zReal * zReal;
                double i2 = zImag * zImag;
                double i3 = zImag * zImag * zImag;
                double i4 = zImag * zImag * zImag * zImag;
                double i5 = zImag * zImag * zImag * zImag * zImag;

                zImag = 5.0f * r4 * zImag - 10.0f * r2 * i3 + i5 + startImag;
                zReal = r5 -10.0f * r3 * i2 + 5.0f * zReal * i4 + startReal;

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


bool DP_x86_Mr::is_valid()
{
    return true;
}

