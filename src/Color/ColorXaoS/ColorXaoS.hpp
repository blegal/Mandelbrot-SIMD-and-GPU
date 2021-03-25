#ifndef _ColorXaoS_
#define _ColorXaoS_

#include "../ColorMap.hpp"

class ColorXaoS : public ColorMap {
public:

    ColorXaoS(int max_iters = 255);

    void setIters(int max_iters);

    virtual ~ColorXaoS();
};

#endif // _ColorSmooth_
