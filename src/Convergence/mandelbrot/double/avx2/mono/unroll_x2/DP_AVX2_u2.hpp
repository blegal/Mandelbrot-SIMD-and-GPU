#ifndef _DP_AVX2_u2_
#define _DP_AVX2_u2_

#include "Convergence/Convergence.hpp"

#define DPP_CONV_STEP 8

class DP_AVX2_u2 : public Convergence {

public:

    DP_AVX2_u2();

    DP_AVX2_u2(ColorMap* _colors, int _max_iters);

    ~DP_AVX2_u2();

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();

};

#endif
