#include "ColorHSVLoop.hpp"


ColorHSVLoop::ColorHSVLoop(int max_iters) : ColorMap("HSV_LOOP")
{
    setIters(max_iters);
}


void ColorHSVLoop::setIters(int max_iters)
{
    MAX = max_iters; // memorisation du nombre d'iterations max.
    for (int i=0; i < MAX; ++i) {
        colors[i] = ColorUtils::HSVToRGB((i)%MAX, 230, 255);
    }
    colors[MAX-1] = sf::Color(0, 0, 0);
//    colors[MAX] = sf::Color(0, 0, 0);
}


ColorHSVLoop::~ColorHSVLoop()
{

}
