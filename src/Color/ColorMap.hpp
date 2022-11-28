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
#ifndef _ColorMap_
#define _ColorMap_

#include <SFML/Graphics.hpp>
#include "ColorUtils.hpp"
#include <array>
#include <iostream>
#include <cassert>

#define VARIATION 0// (MAX/8)
#define ITER ((i+VARIATION)%MAX)

class ColorMap {
public:

    ColorMap(std::string str);

    virtual ~ColorMap();

    virtual void setIters(const int max_iters);

    virtual sf::Color getColor(const int iterations);

    virtual std::string name();

    virtual void colorize(const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr, sf::Image& image);

protected:
    std::array<sf::Color, 65536> colors;

    int MAX;

    std::string _name;
};

#endif // _ColorMap_
