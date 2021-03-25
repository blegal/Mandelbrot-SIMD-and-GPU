#ifndef _ColorLerpBlueRed_
#define _ColorLerpBlueRed_

#include "../ColorMap.hpp"

class ColorLerpBlueRed : public ColorMap {
public:

  ColorLerpBlueRed(int max_iters = 255);

  void setIters(int max_iters);

  virtual ~ColorLerpBlueRed();
};

#endif // _ColorLerpBlueRed_
