#ifndef _ColorSmoothLoop_
#define _ColorSmoothLoop_

#include "../ColorMap.hpp"


class ColorSmoothLoop : public ColorMap {
public:

    ColorSmoothLoop(int max_iters = 255);

    void setIters(int max_iters);

    virtual ~ColorSmoothLoop();
};

#endif // _ColorSmoothLoop_
