#ifndef _QP_x86_
#define _QP_x86_

#include <SFML/Graphics.hpp>
#include <vector>

#include "Convergence/Convergence.hpp"

class QP_x86 : public Convergence {

public:

    QP_x86();

    QP_x86(ColorMap* _colors, int _max_iters);

  ~QP_x86( );

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, sf::Image& image);
    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();

};

#endif
