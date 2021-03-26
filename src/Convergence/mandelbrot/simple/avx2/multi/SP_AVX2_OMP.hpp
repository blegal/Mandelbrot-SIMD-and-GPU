#ifndef _SP_AVX2_OMP_
#define _SP_AVX2_OMP_

#include "Convergence/Convergence.hpp"

class SP_AVX2_OMP : public Convergence {

public:

    SP_AVX2_OMP();

    SP_AVX2_OMP(ColorMap *_colors, int _max_iters);

    ~SP_AVX2_OMP();

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();
};

#endif
