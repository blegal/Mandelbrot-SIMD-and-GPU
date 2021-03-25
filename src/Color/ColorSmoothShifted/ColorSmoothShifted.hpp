#ifndef _ColorSmoothShifted_
#define _ColorSmoothShifted_

#include "../ColorMap.hpp"

class ColorSmoothShifted : public ColorMap {
public:

    ColorSmoothShifted(int max_iters = 255);

    void setIters(int max_iters);

    virtual ~ColorSmoothShifted();
};

#endif // _ColorSmoothShifted_
