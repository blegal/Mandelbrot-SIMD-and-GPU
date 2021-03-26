#ifndef _FP_Q18_14_x86_
#define _FP_Q18_14_x86_

#include <SFML/Graphics.hpp>
#include "Convergence/Convergence.hpp"
#include "Convergence/mandelbrot/fixed/fixed_point.hpp"

class FP_Q18_14_x86 : public Convergence {

public:

    FP_Q18_14_x86();

    FP_Q18_14_x86(ColorMap* _colors, int _max_iters);

    ~FP_Q18_14_x86();

    virtual void updateImage(const long double d_zoom, const long double d_offsetX, const long double d_offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    bool is_valid();
};

#endif
