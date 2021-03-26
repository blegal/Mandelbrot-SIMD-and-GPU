#ifndef _DP_x86_N_
#define _DP_x86_N_

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <thread>

#include "Convergence/Convergence.hpp"

class DP_x86_N : public Convergence {

public:

    DP_x86_N();

    DP_x86_N(ColorMap* _colors, int _max_iters);

    ~DP_x86_N( );

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();
};

#endif
