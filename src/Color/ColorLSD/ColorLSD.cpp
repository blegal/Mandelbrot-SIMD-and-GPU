#include "ColorLSD.hpp"


ColorLSD::ColorLSD(int max_iters) : ColorMap("LSD")
{
    setIters(max_iters);
}


void ColorLSD::setIters(int max_iters)
{
    MAX = max_iters; // memorisation du nombre d'iterations max.
    for (int i = 0; i < MAX; i += 1) {
        colors[i] = ColorUtils::HSVToRGB((255*ITER)%MAX, 255, 255);
    }
    colors[MAX-1] = sf::Color(0, 0, 0);
}


ColorLSD::~ColorLSD()
{

}
