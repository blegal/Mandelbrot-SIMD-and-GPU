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
#include "ColorMap.hpp"


ColorMap::ColorMap(std::string str)
{
	_name = str;
}


ColorMap::~ColorMap()
{

}


void ColorMap::setIters(const int max_iters)
{
    std::cout << "Oups on est dans ColorMap::setIters()" << std::endl;
    exit( EXIT_FAILURE );
}


sf::Color ColorMap::getColor(const int iterations)
{
    assert( iterations <= MAX );
    return colors[iterations];
}


std::string ColorMap::name()
{
    return _name;
}


void ColorMap::colorize(
        const int IMAGE_WIDTH,
        const int IMAGE_HEIGHT,
              float* ptr,
        sf::Image& image)
{

#if 0
    for(uint32_t p = 0; p < IMAGE_WIDTH * IMAGE_HEIGHT -1; p += 1 )
    {
        const float Z0 = ptr[p  ];
        const float Z1 = ptr[p+1];
        const float V  = Z0 - Z1;
        ptr[p] = (V > 0.0f) ? V : 0.0f;
    }

    float minf = 65536.0f;
    float maxf = 0.0f;
    for(uint32_t p = 0; p < IMAGE_WIDTH * IMAGE_HEIGHT; p += 1 )
    {
        minf = (minf < ptr[p]) ? minf : ptr[p];
        maxf = (maxf > ptr[p]) ? maxf : ptr[p];
    }

    for(uint32_t p = 0; p < IMAGE_WIDTH * IMAGE_HEIGHT; p += 1 )
    {
        ptr[p] = (ptr[p] - minf) * (((float)MAX)/maxf);
    }
#endif


    for(int32_t y = 0; y < IMAGE_HEIGHT; y += 1 )
    {
        for(int32_t x = 0; x < IMAGE_WIDTH; x += 1 )
        {
            const uint32_t value = (*ptr++);
            const sf::Color c    = getColor( value );
            image.setPixel(x, y, c);
        }
    }
}
