#ifndef _SP_NEON_OMP_
#define _SP_NEON_OMP_

#include "Convergence/Convergence.hpp"

class SP_NEON_OMP : public Convergence {
public:
    SP_NEON_OMP();

    SP_NEON_OMP(ColorMap *_colors, int _max_iters);

    ~SP_NEON_OMP();

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();
};

#endif
