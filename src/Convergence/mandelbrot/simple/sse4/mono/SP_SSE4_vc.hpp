#ifndef _SP_SSE4_vc_
#define _SP_SSE4_vc_

#include "Convergence/Convergence.hpp"

class SP_SSE4_vc : public Convergence {
public:
    SP_SSE4_vc();

    SP_SSE4_vc(ColorMap *_colors, int _max_iters);

    ~SP_SSE4_vc();

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();
};

#endif
