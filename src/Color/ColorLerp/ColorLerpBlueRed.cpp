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
