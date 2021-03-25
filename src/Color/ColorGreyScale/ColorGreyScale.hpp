#ifndef _ColorGreyScale_
#define _ColorGreyScale_

#include "../ColorMap.hpp"

class ColorGreyScale : public ColorMap {
public:

    ColorGreyScale(int max_iters = 255);

    virtual void setIters(int max_iters);

    virtual ~ColorGreyScale();
};

#endif // _ColorGreyScale_
