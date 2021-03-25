#include "Convergence.hpp"


Convergence::Convergence(std::string value)
{
    _name = value;
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
        resu += "Convergence [\e[0;32m" + align_left (name(),   "JU_SP_AVX512_OMP") + "\e[0m] :";
    else
        resu += "Convergence [\e[0;31m" + align_left (name(),   "JU_SP_AVX512_OMP") + "\e[0m] :";
    resu += " Format = "    + align_right(dataFormat, "double") + " |";
    resu += " SIMD = "      + align_right(modeSIMD,         "none") + " |";
    resu += " OpenMP = "    + align_right(modeOPENMP,    "disable") + " |";
    resu += " OTHER = "     + align_right(OTHER,       "unroll 4x") + " |";
    return resu;
}
