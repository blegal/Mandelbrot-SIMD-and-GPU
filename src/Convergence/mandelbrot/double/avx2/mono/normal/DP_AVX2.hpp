#ifndef _DP_AVX2_
#define _DP_AVX2_

#include "Convergence/Convergence.hpp"

#define CONV_STEP 4

class DP_AVX2 : public Convergence {

public:

    DP_AVX2();

    DP_AVX2(ColorMap* _colors, int _max_iters);

    ~DP_AVX2();

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();
};

#endif
