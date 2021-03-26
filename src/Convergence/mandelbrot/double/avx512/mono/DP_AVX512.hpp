#ifndef _DP_AVX512_
#define _DP_AVX512_

#include "Convergence/Convergence.hpp"

class DP_AVX512 : public Convergence {

public:

    DP_AVX512();

    DP_AVX512(ColorMap *_colors, int _max_iters);

    ~DP_AVX512();

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();

};

#endif
