#include "JU_SP_x86.hpp"


JU_SP_x86::JU_SP_x86() : Convergence("JU_SP_x86")
{
    fractal     = "julia";
    dataFormat  = "float";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}


JU_SP_x86::JU_SP_x86(ColorMap* _colors, int _max_iters) : Convergence("JU_SP_x86") {
    colors      = _colors;
    max_iters   = _max_iters;

    fractal     = "julia";
    dataFormat  = "float";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
}


JU_SP_x86::~JU_SP_x86( )
{

}


unsigned int JU_SP_x86::process(const float startReal, const float startImag, unsigned int max_iters)  {
    float zReal = startReal;
    float zImag = startImag;

    float cReal = 0.25f;
    float cImag = 0.00f;

    for (unsigned int counter = 0; counter < max_iters; counter++) {
        float r2 = zReal * zReal;
        float i2 = zImag * zImag;
        zImag = 2.0f * zReal * zImag + cImag;
        zReal = r2 - i2 + cReal;
        if ( (r2 + i2) > 4.0f) {
            return counter;
        }
    }
    return max_iters - 1;
}


void JU_SP_x86::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
    const float f_zoom    = (float)_zoom;
    const float f_offsetX = (float)_offsetX;
    const float f_offsetY = (float)_offsetY;

    for (int y = 0; y < IMAGE_HEIGHT; y++)
    {
        float imag = f_offsetY - IMAGE_HEIGHT / 2.0f * f_zoom + (y * f_zoom);
        float real = f_offsetX - IMAGE_WIDTH  / 2.0f * f_zoom;
        for (int x = 0; x < IMAGE_WIDTH;  x++)
        {
            uint32_t value = process(real, imag, max_iters);
            (*ptr++) = value;
            real += f_zoom;
        }
    }
}


bool JU_SP_x86::is_valid()
{
    return true;
}
