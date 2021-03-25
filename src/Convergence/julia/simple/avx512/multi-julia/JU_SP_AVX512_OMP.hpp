#ifndef _JU_SP_AVX512_OMP_
#define _JU_SP_AVX512_OMP_

#include "Convergence/Convergence.hpp"

#if defined(__AVX2__)
    #include "vectclass/vectorclass.h"  // library for INTEL AVX2/AVX512 procesors
#endif

class JU_SP_AVX512_OMP : public Convergence {
public:

    JU_SP_AVX512_OMP();

    JU_SP_AVX512_OMP(ColorMap *_colors, int _max_iters);

    ~JU_SP_AVX512_OMP();

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();
};

#endif
