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
