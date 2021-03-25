#ifndef _SP_AVX2_OMP_u2_
#define _SP_AVX2_OMP_u2_

#include "Convergence/Convergence.hpp"

class SP_AVX2_OMP_u2 : public Convergence {

public:

    SP_AVX2_OMP_u2();

    SP_AVX2_OMP_u2(ColorMap* _colors, int _max_iters);

    ~SP_AVX2_OMP_u2();

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();
};

#endif
