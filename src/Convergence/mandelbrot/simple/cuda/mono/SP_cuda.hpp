#if defined(__CUDACC__)
#ifndef _SP_cuda_
#define _SP_cuda_
/*
 *
 *
 *##############################################################################
 *
 *
 */
#include "Convergence/Convergence.hpp"
/*
 *
 *
 *##############################################################################
 *
 *
 */
class SP_cuda : public Convergence {
public:
    SP_cuda();

    SP_cuda(ColorMap* _colors, int _max_iters);

    ~SP_cuda();

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();

private:
    //
    // Buffers to hold data.
    //
    int32_t* gpu_mBuffer;

    int32_t* host_mBuffer;
    float    host_mZoom;
    float    host_mOffsetX;
    float    host_mOffsetY;
    int32_t  host_mWidth;
    int32_t  host_mHeight;
    int32_t  host_mIters;
};
#endif
#endif
