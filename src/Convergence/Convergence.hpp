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
#ifndef CONVERGENCE_H_
#define CONVERGENCE_H_

#include <SFML/Graphics.hpp>
#include "../Color/ColorMap.hpp"

class Convergence {
protected:
    ColorMap* colors;
    int max_iters;
    std::string _name;

    std::string fractal;
    std::string dataFormat;
    std::string modeSIMD;
    std::string modeOPENMP;
    std::string OTHER;

public:
    Convergence(const std::string value);
    Convergence(const std::string _name_, const std::string _fractal_);

    virtual ~Convergence();

    virtual void updateImage(const long double d_zoom, const long double d_offsetX, const long double d_offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr) = 0;

    std::string name();

    void setColor(ColorMap* colorizer);

    void setIters(const unsigned int value);

    std::string toString();
    std::string toShortString();

    virtual bool is_valid() = 0;
};

#endif
