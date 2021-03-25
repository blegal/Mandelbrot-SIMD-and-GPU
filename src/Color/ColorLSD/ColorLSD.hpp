#ifndef _ColorLoop_
#define _ColorLoop_

#include "../ColorMap.hpp"


class ColorLSD : public ColorMap {
public:

    ColorLSD(int max_iters = 255);

    void setIters(int max_iters);

    virtual ~ColorLSD();
};

#endif // _ColorLoop_
