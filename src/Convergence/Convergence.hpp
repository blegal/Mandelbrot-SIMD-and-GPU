#ifndef CONVERGENCE_H_
#define CONVERGENCE_H_

#include <SFML/Graphics.hpp>
#include "../Color/ColorMap.hpp"

class Convergence {
protected:
    ColorMap* colors;
    int max_iters;
    std::string _name;

    std::string fractal;
    std::string dataFormat;
    std::string modeSIMD;
    std::string modeOPENMP;
    std::string OTHER;

public:
    Convergence(const std::string value);
    Convergence(const std::string _name_, const std::string _fractal_);

    virtual ~Convergence();

    virtual void updateImage(const long double d_zoom, const long double d_offsetX, const long double d_offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr) = 0;

    std::string name();

    void setColor(ColorMap* colorizer);

    void setIters(const unsigned int value);

    std::string toString();
    std::string toShortString();

    virtual bool is_valid() = 0;
};

#endif
