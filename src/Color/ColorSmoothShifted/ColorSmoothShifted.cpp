#include "ColorSmoothShifted.hpp"
#include <cmath>

ColorSmoothShifted::ColorSmoothShifted(int max_iters) : ColorMap("SMOOTH_SHIFTED")
{
    setIters(max_iters);
}

void ColorSmoothShifted::setIters(int max_iters)
{
    MAX = max_iters; // memorisation du nombre d'iterations max.

    for (int i=0; i <= MAX; ++i) {
        const float t = (float)i/(float)max_iters;
        const int g   = (int)std::round( 9.f * (1.0f-t) * t * t * t               * 255.0f);
        const int r   = (int)std::round(15.f * (1.0f-t) * (1.0f-t) * t * t        * 255.0f);
        const int b   = (int)std::round(8.5f * (1.0f-t) * (1.0f-t) * (1.0f-t) * t * 255.0f);
        colors[i] = sf::Color(r, g, b);
    }
}

ColorSmoothShifted::~ColorSmoothShifted()
{

}
