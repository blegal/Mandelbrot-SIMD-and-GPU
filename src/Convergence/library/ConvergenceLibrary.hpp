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
#ifndef _ConvergenceLibrary_
#define _ConvergenceLibrary_

#include <vector>
#include <map>
#include <string>

#include "../Convergence.hpp"


class ConvergenceLibrary {
public:
    std::vector<            Convergence*> list;
private:
    std::map   <std::string,Convergence*> dico;
    std::map   <std::string,int         > indx;

    int counter;

public:

    ConvergenceLibrary();

    virtual ~ConvergenceLibrary();

    Convergence* get(std::string name);

    Convergence* get(int num);

    Convergence* get();

    void next();

    void previous();

    int32_t size();

    virtual void listAllModules();

};

#endif // _ColorMap_
