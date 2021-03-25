
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
