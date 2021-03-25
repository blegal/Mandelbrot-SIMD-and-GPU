#include "DP_x86_Mme.hpp"

//
// Madame-brot
//

DP_x86_Mme::DP_x86_Mme() : Convergence("DP_x86_Mme", "Mme Brot")
{
    dataFormat  = "double";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}


DP_x86_Mme::DP_x86_Mme(ColorMap* _colors, int _max_iters) : Convergence("DP_x86_Mme", "Mme Brot")
{
    colors      = _colors;
    max_iters   = _max_iters;

    dataFormat  = "double";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}


DP_x86_Mme::~DP_x86_Mme( )
{

}


void DP_x86_Mme::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
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

                double r3 = zReal * zReal * zReal;
                double r2 = zReal * zReal;
                double i2 = zImag * zImag;
                double i3 = zImag * zImag * zImag;

                zImag = 3.0f * r2 * zImag - i3 + startImag;
                zReal = r3 - 3.0f * zReal * i2 + startReal;

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


bool DP_x86_Mme::is_valid()
{
    return true;
}

