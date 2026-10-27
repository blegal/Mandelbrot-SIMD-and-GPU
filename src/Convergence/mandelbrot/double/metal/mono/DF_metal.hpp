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
#ifndef _DF_metal_
#define _DF_metal_
#if defined(_ENABLE_METAL_)
/*
 *
 *
 *##############################################################################
 *
 *
 */
#include "Convergence/metal/MetalEngine.hpp"
/*
 *
 *
 *##############################################################################
 *
 *
 */
//
// Moteur Metal en arithmetique "float-float" (double precision emulee sur GPU)
//
class DF_metal : public MetalEngine {
public:
    DF_metal();

    DF_metal(ColorMap* _colors, int _max_iters);

    ~DF_metal();

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);
};

#endif
#endif
