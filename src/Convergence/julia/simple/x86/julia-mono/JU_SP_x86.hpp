#ifndef _JU_SP_x86_
#define _JU_SP_x86_

#include "Convergence/Convergence.hpp"

class JU_SP_x86 : public Convergence {
public:

    JU_SP_x86();

    JU_SP_x86(ColorMap* _colors, int _max_iters);

    ~JU_SP_x86();

    virtual unsigned int process(const float startReal, const float startImag, unsigned int max_iters);

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();

};

#endif
