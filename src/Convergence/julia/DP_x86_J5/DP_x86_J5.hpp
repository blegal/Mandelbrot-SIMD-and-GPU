#ifndef _DP_x86_J5_
#define _DP_x86_J5_

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <thread>

#include "Convergence/Convergence.hpp"

class DP_x86_J5 : public Convergence {

public:

    DP_x86_J5();

    DP_x86_J5(ColorMap* _colors, int _max_iters);

    ~DP_x86_J5( );

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();
};

#endif
