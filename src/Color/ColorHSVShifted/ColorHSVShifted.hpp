#ifndef _ColorHSVShifted_
#define _ColorHSVShifted_

#include "../ColorMap.hpp"

class ColorHSVShifted : public ColorMap {
public:

    ColorHSVShifted(int max_iters = 255);

    void setIters(int max_iters);

    virtual ~ColorHSVShifted();
};

#endif // _ColorHSVShifted_
