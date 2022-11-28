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
#include "Convergence.hpp"


Convergence::Convergence(const std::string value)
{
    _name = value;
}


Convergence::Convergence(const  std::string _name_, const std::string _fractal_)
{
    _name   = _name_;
    fractal = _fractal_;
}


Convergence::~Convergence()
{

}


std::string Convergence::name()
{
    return _name;
}


void Convergence::setColor(ColorMap* colorizer)
{
    colors = colorizer;
}


void Convergence::setIters(const unsigned int value)
{
    max_iters = value;
}


std::string align_right(std::string str, std::string model)
{
    std::string r;
    for(uint32_t i = str.length(); i < model.length(); i += 1 )
        r += " ";
    r += str;
    return r;
}


std::string align_left(std::string str, std::string model)
{
    std::string r;
    r += str;
    for(uint32_t i = str.length(); i < model.length(); i += 1 )
        r += " ";
    return r;
}


std::string Convergence::toString()
{
    std::string resu;
    if( is_valid() )
        resu += "[\e[0;32m" + align_left (name(),   "JU_SP_AVX512_OMP") + "\e[0m] :";
    else
        resu += "[\e[0;31m" + align_left (name(),   "JU_SP_AVX512_OMP") + "\e[0m] :";
    resu += " Format = "    + align_right(dataFormat, "double") + " |";
    resu += " SIMD = "      + align_right(modeSIMD,         "AVX512") + " |";
    resu += " OpenMP = "    + align_right(modeOPENMP,    "disable") + " |";
    resu += " OTHER = "     + align_right(OTHER,       "unroll 4x") + " |";
    return resu;
}

std::string Convergence::toShortString()
{
    std::string resu;
    if( is_valid() )
        resu += "\e[0;32m" + align_left (name(),   "JU_SP_AVX512_OMP") + "\e[0m|";
    else
        resu += "\e[0;31m" + align_left (name(),   "JU_SP_AVX512_OMP") + "\e[0m|";
    resu += align_right(dataFormat, "double"   ) + "|";
    resu += align_right(modeSIMD,   "AVX512"   ) + "|";
    resu += align_right(modeOPENMP, "disable"  ) + "|";
    resu += align_right(OTHER,      "unroll 4x") + "|";
    return resu;
}
