#ifndef _SP_metal_
#define _SP_metal_
#if defined(__APPLE__)
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
#include "Metal.hpp"
/*
 *
 *
 *##############################################################################
 *
 *
 */
class SP_metal : public Convergence {
public:
    SP_metal();

    SP_metal(ColorMap* _colors, int _max_iters);

    ~SP_metal();

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();

private:
    MTL::Device *_mDevice;

    //
    // The compute pipeline generated from the compute kernel in the .metal shader file.
    //
    MTL::ComputePipelineState *_mAddFunctionPSO;

    //
    // The command queue used to pass commands to the device.
    //
    MTL::CommandQueue *_mCommandQueue;

    //
    // Buffers to hold data.
    //
    MTL::Buffer *_mBuffer;
    MTL::Buffer *_mZoom;
    MTL::Buffer *_mOffsetX;
    MTL::Buffer *_mOffsetY;
    MTL::Buffer *_mWidth;
    MTL::Buffer *_mHeight;
    MTL::Buffer *_mIters;

};

#endif
#endif
