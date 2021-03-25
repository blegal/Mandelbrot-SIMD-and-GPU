#ifndef _ColorLibrary_
#define _ColorLibrary_

#include <vector>
#include <map>
#include <string>

#include "../ColorMap.hpp"

#include "../ColorGreyScale/ColorGreyScale.hpp"

#include "../ColorHSV/ColorHSV.hpp"
#include "../ColorHSVLoop/ColorHSVLoop.hpp"
#include "../ColorHSVShifted/ColorHSVShifted.hpp"

#include "../ColorLerp/ColorLerpBlueRed.hpp"
#include "../ColorLSD/ColorLSD.hpp"

#include "../ColorSetCandy/ColorSetCandy.hpp"


#include "../ColorSmooth/ColorSmooth.hpp"
#include "../ColorSmoothLoop/ColorSmoothLoop.hpp"
#include "../ColorSmoothShifted/ColorSmoothShifted.hpp"

class ColorLibrary {
private:
    std::vector<            ColorMap*> list;
    std::map   <std::string,ColorMap*> dico;
    std::map   <std::string,int      > indx;

    int counter;

public:

    ColorLibrary();

    virtual ~ColorLibrary();

    ColorMap* get(std::string name);
    ColorMap* get(int num);
    ColorMap* get();

    void next();

    void previous();

};

#endif // _ColorMap_
