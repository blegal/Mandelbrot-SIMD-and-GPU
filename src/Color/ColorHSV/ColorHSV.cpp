#include "ColorHSV.hpp"


ColorHSV::ColorHSV(int max_iters) : ColorMap("HSV")
{
    setIters(max_iters);
}


void ColorHSV::setIters(int max_iters)
{
    MAX = max_iters;
    for (int i=0; i < MAX; ++i)
    {
        colors[i] = ColorUtils::HSVToRGB(255.0f * ((float)ITER)/((float)MAX), 255, 255);
    }
    colors[MAX-1] = sf::Color(0, 0, 0);
//  colors[MAX  ] = sf::Color(0, 0, 0);
}


ColorHSV::~ColorHSV()
{

}
