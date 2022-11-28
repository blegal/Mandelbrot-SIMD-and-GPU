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
#ifndef COLOR_UTILS_H_
#define COLOR_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

class ColorUtils {
public:
	static sf::Color HSVToRGB(int h, int s, int v) {
		int r, g, b;
	  unsigned char reg, rem, p, q, t;
	  if (s == 0) {
	      r = v;
	      g = v;
	      b = v;
	  } else {
	    reg = h / 43;
	    rem = (h - (reg * 43)) * 6;

	    p = (v * (255 - s)) >> 8;
	    q = (v * (255 - ((s * rem) >> 8))) >> 8;
	    t = (v * (255 - ((s * (255 - rem)) >> 8))) >> 8;

	    switch (reg) {
	    case 0: //   0° <= H <  60°
	      r = v; g = t; b = p;
	      break;
	    case 1: //  60° <= H < 120°
	      r = q; g = v; b = p;
	      break;
	    case 2: // 120° <= H < 180°
	      r = p; g = v; b = t;
	      break;
	    case 3: // 180° <= H < 240°
	      r = p; g = q; b = v;
	      break;
	    case 4: // 240° <= H < 300°
	      r = t; g = p; b = v;
	      break;
	    default: // 300° <= H < 360°
	      r = v; g = p; b = q;
	      break;
	    }
	  }
	  return sf::Color(r, g, b);
	}
};

#endif /*COLOR_UTILS_H_*/
