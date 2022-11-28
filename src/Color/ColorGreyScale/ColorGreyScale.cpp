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
#include "ColorGreyScale.hpp"


ColorGreyScale::ColorGreyScale(int max_iters) : ColorMap("GREYSCALE")
{
    setIters(max_iters);
}


void ColorGreyScale::setIters(int max_iters)
{

    MAX = max_iters;

    for (int32_t i = 0; i < MAX; ++i)
    {
        const float t = 255.0f * ((float)i/(float)max_iters);
        colors[i] = sf::Color(t, t, t);
    }
    colors[MAX-1] = sf::Color(0, 0, 0);
}


ColorGreyScale::~ColorGreyScale()
{

}
