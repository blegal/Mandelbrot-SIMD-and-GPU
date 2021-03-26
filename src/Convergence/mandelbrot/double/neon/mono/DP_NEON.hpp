#ifndef _DP_NEON_
#define _DP_NEON_

#include "Convergence/Convergence.hpp"

class DP_NEON : public Convergence {
public:
    DP_NEON();

    DP_NEON(ColorMap *_colors, int _max_iters);

    ~DP_NEON();

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();
};

#endif
