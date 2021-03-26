#ifndef _SP_AVX512_
#define _SP_AVX512_

#include "Convergence/Convergence.hpp"

class SP_AVX512 : public Convergence {
public:
    SP_AVX512();

    SP_AVX512(ColorMap *_colors, int _max_iters);

    ~SP_AVX512();

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();
};

#endif
