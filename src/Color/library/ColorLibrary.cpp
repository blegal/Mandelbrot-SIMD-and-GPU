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
#include "ColorLibrary.hpp"


ColorLibrary::ColorLibrary()
{
    counter = 0;

    //
    //
    //
    list.push_back( new ColorGreyScale    () );
    list.push_back( new ColorHSV          () );
    list.push_back( new ColorHSVLoop      () );
    list.push_back( new ColorHSVShifted   () );
    list.push_back( new ColorLerpBlueRed  () );
    list.push_back( new ColorLSD          () );
    list.push_back( new ColorSetCandy     () );
    list.push_back( new ColorSmooth       () );
    list.push_back( new ColorSmoothLoop   () );
    list.push_back( new ColorSmoothShifted() );

    //
    //
    //
    for(uint32_t i=0; i<list.size(); i++){
        dico[ list[i]->name() ] = list[i];
        indx[ list[i]->name() ] =      i ;
    }  
}


ColorLibrary::~ColorLibrary()
{
    for(uint32_t i = 0; i < list.size(); i++)
        delete list[i];
}


ColorMap* ColorLibrary::get(std::string name)
{
    counter = indx[name];
    return get();
}


ColorMap* ColorLibrary::get(int num)
{
    counter = num;
    return get();
}


ColorMap* ColorLibrary::get(){
    printf("(II) Switching to ColorLibrary [%s]\n", list[counter]->name().c_str());
    return list[counter];
}


void ColorLibrary::next()
{
    counter = (counter + 1) % list.size();
    //printf("COUNTER %d / %lu\n", counter, list.size());
    //return get();
}


void ColorLibrary::previous()
{
    counter = (counter + list.size() - 1) % list.size();
    //printf("COUNTER %d / %lu\n", counter, list.size());
    //return get();
}
