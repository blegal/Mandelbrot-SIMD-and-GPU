#include "DP_x86_J5.hpp"


DP_x86_J5::DP_x86_J5() : Convergence("DP_x86_J5", "Julia")
{
    dataFormat  = "double";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}


DP_x86_J5::DP_x86_J5(ColorMap* _colors, int _max_iters) : Convergence("DP_x86_J5", "Julia")
{
    colors      = _colors;
    max_iters   = _max_iters;

    dataFormat  = "double";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}


DP_x86_J5::~DP_x86_J5( ){

}


void DP_x86_J5::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
    double offsetX = _offsetX;
    double offsetY = _offsetY;
    double zoom    = _zoom;

    for (int y = 0; y < IMAGE_HEIGHT; y++)
    {
        for (int x = 0; x < IMAGE_WIDTH;  x++)
        {
            int value    = max_iters - 1;

            double zReal = offsetX - IMAGE_WIDTH  / 2.0f * zoom + (x * zoom);   //xn
            double zImag = offsetY - IMAGE_HEIGHT / 2.0f * zoom + (y * zoom);   //yn

            double startReal = -0.835;
            double startImag = -0.2321;

            for (unsigned int counter = 0; counter < max_iters; counter++)
            {
                ////////////////////////////////////////////////////////////////////////////////////////////////////////
                double r2 = zReal * zReal;
                double i2 = zImag * zImag;
                zImag = 2.0f * zReal * zImag + startImag;   //yn
                zReal = (r2 - i2) + startReal;              //xn
                if ( (r2 + i2) > 4.0f) {
                    value = counter;
                    break;
                }
                ////////////////////////////////////////////////////////////////////////////////////////////////////////
            }
            (*ptr++) = value;
        }
    }
}


bool DP_x86_J5::is_valid()
{
    return true;
}

