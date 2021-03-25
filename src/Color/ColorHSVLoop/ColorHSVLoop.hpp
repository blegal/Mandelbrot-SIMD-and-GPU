#ifndef _ColorHSV_Loop_
#define _ColorHSV_Loop_

#include "../ColorMap.hpp"


class ColorHSVLoop : public ColorMap {
public:

    ColorHSVLoop(int max_iters = 255);

    void setIters(int max_iters);

    virtual ~ColorHSVLoop();
};

#endif // _ColorHSV_Loop_
