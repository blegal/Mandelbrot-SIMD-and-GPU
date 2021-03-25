#ifndef _DP_x86_OMP_
#define _DP_x86_OMP_

#include "Convergence/Convergence.hpp"

class DP_x86_OMP : public Convergence {

public:

    DP_x86_OMP();

    DP_x86_OMP(ColorMap* _colors, int _max_iters);

    ~DP_x86_OMP( );

    virtual unsigned int process(const double startReal, const double startImag, unsigned int max_iters);

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();

};

#endif
