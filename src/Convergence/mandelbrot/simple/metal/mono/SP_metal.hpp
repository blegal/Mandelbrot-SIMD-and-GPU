/*
 *  Copyright (c) 2026-... Bertrand LE GAL
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held
 *  liable for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute
 *  it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment
 *  in the product documentation would be appreciated but
 *  is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any
 *  source distribution.
 *
 */
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
