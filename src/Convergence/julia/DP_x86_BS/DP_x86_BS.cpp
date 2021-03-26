#include "DP_x86_BS.hpp"

//
// https://en.wikipedia.org/wiki/Burning_Ship_fractal
//

DP_x86_BS::DP_x86_BS() : Convergence("DP_x86_BS", "BurningShip")
{
    dataFormat  = "double";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}


DP_x86_BS::DP_x86_BS(ColorMap* _colors, int _max_iters) : Convergence("DP_x86_BS", "BurningShip")
{
    colors      = _colors;
    max_iters   = _max_iters;

    dataFormat  = "double";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}


DP_x86_BS::~DP_x86_BS( ){

}


void DP_x86_BS::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
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
                zImag = abs(zImag);
                zReal = abs(zReal);
                double r2 = zReal * zReal;
                double i2 = zImag * zImag;
                zImag = 2.0f * zReal * zImag + startImag;
                zReal = r2 - i2 + startReal;
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


bool DP_x86_BS::is_valid()
{
    return true;
}

