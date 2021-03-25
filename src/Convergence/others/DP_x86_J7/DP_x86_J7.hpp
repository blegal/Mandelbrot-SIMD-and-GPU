#ifndef _DP_x86_J7_
#define _DP_x86_J7_

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <thread>
#include "immintrin.h"

#include "Convergence/Convergence.hpp"

class DP_x86_J7 : public Convergence {

public:

    DP_x86_J7();

    DP_x86_J7(ColorMap* _colors, int _max_iters);

    ~DP_x86_J7( );

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();
};

#endif