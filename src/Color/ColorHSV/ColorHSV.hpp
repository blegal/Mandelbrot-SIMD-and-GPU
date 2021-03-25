#ifndef _ColorHSV_
#define _ColorHSV_

#include "../ColorMap.hpp"


class ColorHSV : public ColorMap {
public:

    ColorHSV(int max_iters = 255);

    void setIters(int max_iters);

    virtual ~ColorHSV();
};

#endif // _ColorHSV_
