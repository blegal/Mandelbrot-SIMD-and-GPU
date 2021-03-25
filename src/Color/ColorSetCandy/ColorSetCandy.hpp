#ifndef _ColorSetCandy_
#define _ColorSetCandy_

#include "../ColorMap.hpp"

class ColorSetCandy : public ColorMap {
public:

    ColorSetCandy(int max_iters = 255);

    virtual void setIters(int max_iters);

    virtual ~ColorSetCandy();
};

#endif // ColorSetCandy
