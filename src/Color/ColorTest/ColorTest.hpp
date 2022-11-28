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
#ifndef _ColorTest_
#define _ColorTest_

#include "../ColorMap.hpp"
#include <SFML/Graphics.hpp>

class ColorTest : public ColorMap {
public:

    ColorTest(int max_iters = 255) : ColorMap("TEST"){
        setIters(max_iters);
    }

    void setIters(int max_iters) {

        MAX = max_iters; // memorisation du nombre d'iterations max.

        for (int i=0; i < MAX; ++i) {
            const float t = (float)i/100.0 ;
            const int r   = (int)round( 9.f * (1.0f-t) * t * t * t               * 255.0f);
            const int g   = (int)round(15.f * (1.0f-t) * (1.0f-t) * t * t        * 255.0f);
            const int b   = (int)round(8.5f * (1.0f-t) * (1.0f-t) * (1.0f-t) * t * 255.0f);
            colors[i] = sf::Color(r, g, b);
        }
        colors[MAX-1] = sf::Color(0, 0, 0);
    }

    virtual ~ColorTest(){

    }

//    virtual sf::Color getColor(int iterations){
//        assert( iterations <= MAX );
//        return colors[iterations];
//    }
};

#endif // _ColorTest_
