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
#include "ColorLerpBlueRed.hpp"


ColorLerpBlueRed::ColorLerpBlueRed(int max_iters) : ColorMap("LERP_BLUE_RED")
{
    setIters(max_iters);
}


void ColorLerpBlueRed::setIters(int max_iters)
{
    MAX = max_iters; // memorisation du nombre d'iterations max.
    sf::Color red(255, 0, 0);
    sf::Color blue(0, 0, 255);
    for (int i=0; i < MAX; ++i) {
#if 0
        colors[i] = ColorUtils::Lerp(red, blue, float(i)/float(MAX));
#endif
    }
    colors[MAX-1] = sf::Color(0, 0, 0);
    colors[MAX  ] = sf::Color(0, 0, 0);
}


ColorLerpBlueRed::~ColorLerpBlueRed()
{

}
