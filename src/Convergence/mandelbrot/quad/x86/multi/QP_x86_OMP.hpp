#ifndef _QP_x86_OMP_
#define _QP_x86_OMP_

#include "Convergence/Convergence.hpp"

class QP_x86_OMP : public Convergence {

public:

    QP_x86_OMP();

    QP_x86_OMP(ColorMap* _colors, int _max_iters);

    ~QP_x86_OMP( );

    virtual uint32_t process(const long double startReal, const long double startImag, uint32_t max_iters);

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, sf::Image& image);

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    bool is_valid();

};
#endif
