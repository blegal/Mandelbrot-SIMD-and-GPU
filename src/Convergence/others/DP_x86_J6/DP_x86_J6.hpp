#ifndef _DP_x86_J6_
#define _DP_x86_J6_

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <thread>
#include "immintrin.h"

#include "Convergence/Convergence.hpp"

class DP_x86_J6 : public Convergence {

public:

    DP_x86_J6();

    DP_x86_J6(ColorMap* _colors, int _max_iters);

    ~DP_x86_J6( );

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();
};

#endif
