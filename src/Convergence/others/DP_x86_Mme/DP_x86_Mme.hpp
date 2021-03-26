#ifndef _DP_x86_Mme_
#define _DP_x86_Mme_

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <thread>

#include "Convergence/Convergence.hpp"

class DP_x86_Mme : public Convergence {

public:

    DP_x86_Mme();

    DP_x86_Mme(ColorMap* _colors, int _max_iters);

    ~DP_x86_Mme( );

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();

};

#endif
