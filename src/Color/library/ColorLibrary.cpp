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
