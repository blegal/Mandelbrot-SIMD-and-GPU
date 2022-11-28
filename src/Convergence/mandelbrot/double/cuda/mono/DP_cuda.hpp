#if defined(_ENABLE_CUDA_) || defined(__NVCC__) || defined(__CUDACC__)
#ifndef _DP_cuda_
#define _DP_cuda_
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
class DP_cuda : public Convergence {
public:
    DP_cuda();

    DP_cuda(ColorMap* _colors, int _max_iters);

    ~DP_cuda();

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();

private:
    //
    // Buffers to hold data.
    //
    int32_t* gpu_mBuffer;

    int32_t* host_mBuffer;
    double   host_mZoom;
    double   host_mOffsetX;
    double   host_mOffsetY;
    int32_t  host_mWidth;
    int32_t  host_mHeight;
    int32_t  host_mIters;
};
#endif
#endif
