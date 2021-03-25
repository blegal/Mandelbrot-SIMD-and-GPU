#include "ColorHSVShifted.hpp"


ColorHSVShifted::ColorHSVShifted(int max_iters) : ColorMap("HSV_SHIFTED")
{
    setIters(max_iters);
}


void ColorHSVShifted::setIters(int max_iters)
{
    MAX = max_iters; // memorisation du nombre d'iterations max.
    for (int i=0; i < MAX; ++i)
    {
        colors[i] = ColorUtils::HSVToRGB(255*(MAX-ITER)/MAX, 255, 255);
    }
    colors[MAX-1] = sf::Color(0, 0, 0);
    colors[MAX] = sf::Color(0, 0, 0);
}


ColorHSVShifted::~ColorHSVShifted()
{

}
