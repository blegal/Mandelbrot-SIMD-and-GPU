#ifndef _SP_NEON_
#define _SP_NEON_

#include "Convergence/Convergence.hpp"

class SP_NEON : public Convergence {
public:
    SP_NEON();

    SP_NEON(ColorMap *_colors, int _max_iters);

    ~SP_NEON();

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();
};

#endif
