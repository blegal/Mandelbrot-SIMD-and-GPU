#ifndef _JU_SP_AVX512_
#define _JU_SP_AVX512_

#include "Convergence/Convergence.hpp"

#if defined(__AVX2__)
    #include "vectclass/vectorclass.h"  // library for INTEL AVX2/AVX512 procesors
#endif

class JU_SP_AVX512 : public Convergence {
public:

    JU_SP_AVX512();

    JU_SP_AVX512(ColorMap *_colors, int _max_iters);

    ~JU_SP_AVX512();

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();
};

#endif
