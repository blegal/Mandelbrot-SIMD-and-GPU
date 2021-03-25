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
