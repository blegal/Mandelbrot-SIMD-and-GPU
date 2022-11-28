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
#ifndef _FP_18b_x86_
#define _FP_18b_x86_

#include <SFML/Graphics.hpp>

#include "Convergence/Convergence.hpp"
#include "Convergence/fixed/fixed_point.hpp"

class FP_18b_x86 : public Convergence {

public:

  FP_18b_x86() : Convergence("FP_18b") {
    dataFormat  = "fixed";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
  }

  FP_18b_x86(ColorMap* _colors, int _max_iters) : Convergence("FP_18b") {
    colors      = _colors;
    max_iters   = _max_iters;

    dataFormat  = "fixed";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "none";
  }

  ~FP_18b_x86()
  {

  }

  void disp_cmp_real(double float_val, fi32_t fixed_val, unsigned int fractionalBitCount)  {
    std::cout << "float = " << float_val << ", fixed = " << fi32_to_double(fixed_val, fractionalBitCount) << std::endl;
  }

#define Q_FORMAT FI_32_27

    virtual void updateImage(const long double d_zoom, const long double d_offsetX, const long double d_offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr) {
    for (int y = 0; y < IMAGE_HEIGHT; y++)
    {
        fi32_t offsetX = double_to_fi32(d_offsetX, Q_FORMAT);
        fi32_t offsetY = double_to_fi32(d_offsetY, Q_FORMAT);
        fi32_t zoom    = double_to_fi32(d_zoom,    Q_FORMAT);

        fi32_t startImag = offsetY  -  IMAGE_HEIGHT/2 * zoom  +  y * zoom;
        fi32_t startReal = offsetX  -  IMAGE_WIDTH/2  * zoom;

        for (int x = 0; x < IMAGE_WIDTH;  x++) {

          int value = max_iters - 1;

          fi32_t zReal = startReal;
          fi32_t zImag = startImag;

          for (int counter = 0; counter < max_iters; counter++)
          {
            //double d_r2 = d_zReal * d_zReal;
              const fi64_t _r2_64 = ((fi64_t)(zReal)) * ((fi64_t)(zReal));
              const fi64_t _r2_    = _r2_64 >> Q_FORMAT;
              const fi32_t _r2     = (_r2_ < MAX_FI_32) ? _r2_ : MAX_FI_32;  // USELESS ?

              //double d_i2 = d_zImag * d_zImag;
              const fi64_t _i2_64 = (((fi64_t)(zImag)) * ((fi64_t)(zImag)));
              const fi64_t _i2_   = _i2_64 >> Q_FORMAT;
              const fi32_t _i2    = (_i2_ < MAX_FI_32) ? _i2_ : MAX_FI_32;   // USELESS ?

              //double d_i2 = d_zReal * d_zImag;
              const fi64_t _ri_64 = (((fi64_t)(zReal)) * ((fi64_t)(zImag)));
              const fi64_t _ri_   = _ri_64 >> Q_FORMAT;
              const fi32_t _ri    = (_ri_ < MAX_FI_32) ? _ri_ : MAX_FI_32;   // USELESS ?
              const fi32_t _2ri   = 2 * _ri;

              zImag = (_2ri      + startImag);
              zReal = (_r2 - _i2 + startReal);

              if ( (_r2 + _i2) > (4 << Q_FORMAT))
              {
                value = counter;
                break;
              }
          }
          (*ptr++) = value;
          startReal = startReal + zoom;
        }
      }
    }


    bool is_valid()
    {
#ifdef __AVX2__
        return true;
#else
        return false;
#endif
    }
};

#endif
