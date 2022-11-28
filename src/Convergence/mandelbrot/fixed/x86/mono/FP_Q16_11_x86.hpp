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
#ifndef _FP_Q16_11_x86_
#define _FP_Q16_11_x86_

#include <SFML/Graphics.hpp>
#include "Convergence/Convergence.hpp"
#include "Convergence/mandelbrot/fixed/fixed_point.hpp"

class FP_Q16_11_x86 : public Convergence {

public:

    FP_Q16_11_x86();

    FP_Q16_11_x86(ColorMap* _colors, int _max_iters);

    ~FP_Q16_11_x86();

    virtual void updateImage(const long double d_zoom, const long double d_offsetX, const long double d_offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    bool is_valid();
};

#endif
