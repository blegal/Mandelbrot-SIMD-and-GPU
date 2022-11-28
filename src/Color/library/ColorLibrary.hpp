/*
 *  Copyright (c) 2026-... Bertrand LE GAL
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held
 *  liable for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute
 *  it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment
 *  in the product documentation would be appreciated but
 *  is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any
 *  source distribution.
 *
 */
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
