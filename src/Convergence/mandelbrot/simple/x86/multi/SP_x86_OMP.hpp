#ifndef _SP_x86_OMP_
#define _SP_x86_OMP_

#include "Convergence/Convergence.hpp"

class SP_x86_OMP : public Convergence {
public:

    SP_x86_OMP();

    SP_x86_OMP(ColorMap* _colors, int _max_iters);

    ~SP_x86_OMP( );

    virtual unsigned int process(const float startReal, const float startImag, unsigned int max_iters);

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();

};

#endif
