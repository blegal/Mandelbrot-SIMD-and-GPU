#ifndef _DP_AVX2_u4_
#define _DP_AVX2_u4_

#include "Convergence/Convergence.hpp"

class DP_AVX2_u4 : public Convergence {

public:

    DP_AVX2_u4();

    DP_AVX2_u4(ColorMap* _colors, int _max_iters);

    ~DP_AVX2_u4();

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();

};

#endif
