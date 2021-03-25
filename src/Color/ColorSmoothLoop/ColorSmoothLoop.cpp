#include "ColorSmoothLoop.hpp"
#include <cmath>

ColorSmoothLoop::ColorSmoothLoop(int max_iters) : ColorMap("SMOOTH_LOOP")
{
    setIters(max_iters);
}

void ColorSmoothLoop::setIters(int max_iters)
{

    MAX = max_iters; // memorisation du nombre d'iterations max.

    for (int i=0; i < MAX; ++i) {
        const float t = (float)i/100.0 ;
        const int r   = (int)std::round( 9.f * (1.0f-t) * t * t * t               * 255.0f);
        const int g   = (int)std::round(15.f * (1.0f-t) * (1.0f-t) * t * t        * 255.0f);
        const int b   = (int)std::round(8.5f * (1.0f-t) * (1.0f-t) * (1.0f-t) * t * 255.0f);
        colors[i] = sf::Color(r, g, b);

    }

    colors[MAX-1] = sf::Color(0, 0, 0);
    colors[MAX  ] = sf::Color(0, 0, 0);

#if 0
    for (int i=0; i < MAX+1; ++i)
    {
        if( i%16 == 0)
            printf("\n");
        printf("0x%1.1X%1.1X%1.1X, ", (colors[i].b >> 4), (colors[i].g >> 4), (colors[i].r >> 4));
    }
    printf("\n");
#endif
}


ColorSmoothLoop::~ColorSmoothLoop()
{

}
