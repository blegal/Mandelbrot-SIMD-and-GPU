#ifndef _Convergence_fp_x86_nfwa_
#define _Convergence_fp_x86_nfwa_

#include <SFML/Graphics.hpp>
#include "../../Convergence.hpp"
#include "fixed_point.hpp"

class Convergence_fp_x86_nfwa : public Convergence {

public:

  Convergence_fp_x86_nfwa() {
  }

  Convergence_fp_x86_nfwa(ColorMap* _colors, int _max_iters){
    colors    = _colors;
    max_iters = _max_iters;
  }

  ~Convergence_fp_x86_nfwa() {
  }

  virtual void updateImage(const double d_zoom, const double d_offsetX, const double d_offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, sf::Image& image) {
    for (int y = 0; y < IMAGE_HEIGHT; y++) {

      fi32_t offsetX = double_to_fi32(d_offsetX, FI_32_28);
      fi32_t offsetY = double_to_fi32(d_offsetY, FI_32_28);

      fi32_t zoom = double_to_fi32(d_zoom, FI_32_28);

      //fi32_t startImag = ((offsetY << FI_32_28) - (((IMAGE_HEIGHT >> 2) * zoom)) + (y * zoom)) << 1;
      double d_startImag = d_offsetY  -  IMAGE_HEIGHT / 2.0f * d_zoom  +  y * d_zoom;
      fi32_t startImag = offsetY  -  IMAGE_HEIGHT/2 * zoom  +  y * zoom;
      //cout << "startImag (float) = " << d_startImag << ", startImag (fixed) = " << fi32_to_double(startImag, FI_32_28) << std::endl;

      double d_startReal = d_offsetX - IMAGE_WIDTH / 2.0f * d_zoom;
      fi32_t startReal = offsetX  -  IMAGE_WIDTH/2 * zoom;
      //cout << "startReal (float) = " << d_startReal << ", startReal (fixed) = " << fi32_to_double(startReal, FI_32_28) << std::endl;
      //sleep(1);

      for (int x = 0; x < IMAGE_WIDTH;  x++) {
        int value = max_iters - 1;

        double d_zReal = d_startReal;
        fi32_t zReal = startReal;
        double d_zImag = d_startImag;
        fi32_t zImag = startImag;

        for (unsigned int counter = 0; counter < max_iters; counter++) {
          double d_r2 = d_zReal * d_zReal;
          fi64_t r2_64 = (fi64_t)zReal*(fi64_t)zReal;
          fi32_t r2 = (fi32_t)(r2_64 >> FI_32_28);
          disp_cmp_real(d_r2, r2, FI_32_28);
          sleep(1);

          double d_i2 = d_zImag * d_zImag;
          fi64_t i2_64 = (fi64_t)zImag*(fi64_t)zImag;
          fi32_t i2 = (fi32_t)(i2_64 >> FI_32_28);

          d_zImag = 2.0f * d_zReal * d_zImag + d_startImag;
          fi64_t z_real_z_imag_64 = (fi64_t)zReal*(fi64_t)zImag;
          fi32_t z_real_z_imag = (fi32_t)(z_real_z_imag_64 >> FI_32_28);
          zImag = 2 * z_real_z_imag  +  startImag;
          d_zReal = d_r2 - d_i2 + d_startReal;
          zReal = r2  -  i2  +  startReal;
          if ( (r2 + i2) > 4) {
            value = counter;
            break;
          }
        }
        image.setPixel(x, y, colors->getColor(value));
        d_startReal += d_zoom;
        startReal = startReal + zoom;
      }
    }
  }
};

#endif
