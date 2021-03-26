#ifndef _FP_Q16_11_x86_
#define _FP_Q16_11_x86_

#include <SFML/Graphics.hpp>
#include "Convergence/Convergence.hpp"
#include "Convergence/mandelbrot/fixed/fixed_point.hpp"

class FP_Q16_11_x86 : public Convergence {

public:

    FP_Q16_11_x86();

    FP_Q16_11_x86(ColorMap* _colors, int _max_iters);

    ~FP_Q16_11_x86();

    virtual void updateImage(const long double d_zoom, const long double d_offsetX, const long double d_offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    bool is_valid();
};

#endif
