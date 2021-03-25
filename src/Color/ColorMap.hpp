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
