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
