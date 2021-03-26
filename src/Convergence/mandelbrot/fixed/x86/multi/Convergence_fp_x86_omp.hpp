#ifndef _Convergence_fp_x86_omp_
#define _Convergence_fp_x86_omp_

//#include "../../../../../../../../../../usr/local/include/SFML/Graphics.hpp"
#include "../../../../Convergence.hpp"
#include "../../fixed_point.hpp"

/*
void disp_real(string name, double float_val, fi32_t fixed_val)  {
  std::cout << name << " : ";
  printf("float =%1.9lf, fixed = %1.9lf\n", float_val, fi32_to_double(fixed_val, FI_32_25));
  //std::cout << name <<  "float = " << float_val << ", fixed = " << fi32_to_double(fixed_val, FI_32_25) << std::endl;
}
*/

class Convergence_fp_x86_omp : public Convergence {

public:

  Convergence_fp_x86_omp() : Convergence("FP_OMP")
  {
    dataFormat  = "fixed";
    modeSIMD    = "none";
    modeOPENMP  = "enable";
    OTHER       = "none";
  }

  Convergence_fp_x86_omp(ColorMap* _colors, int _max_iters) : Convergence("FP_OMP") {
    colors      = _colors;
    max_iters   = _max_iters;

    dataFormat  = "fixed";
    modeSIMD    = "none";
    modeOPENMP  = "enable";
    OTHER       = "none";
  }

  ~Convergence_fp_x86_omp() {
  }

  void disp_cmp_real(double float_val, fi32_t fixed_val, unsigned int fractionalBitCount)  {
    std::cout << "float = " << float_val << ", fixed = " << fi32_to_double(fixed_val, fractionalBitCount) << std::endl;
  }

  virtual void updateImage(const long double d_zoom, const long double d_offsetX, const long double d_offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, sf::Image& image) {
    #pragma omp parallel // on declare une section parallel
    {
      #pragma omp for // on fait du multicoeur
      for (int y = 0; y < IMAGE_HEIGHT; y++) {

        fi32_t offsetX = double_to_fi32(d_offsetX, FI_32_25);
        fi32_t offsetY = double_to_fi32(d_offsetY, FI_32_25);

        fi32_t zoom = double_to_fi32(d_zoom, FI_32_25);

        //double d_startImag = d_offsetY  -  IMAGE_HEIGHT / 2.0f * d_zoom  +  y * d_zoom;
        fi32_t startImag = offsetY  -  IMAGE_HEIGHT/2 * zoom  +  y * zoom;

        //double d_startReal = d_offsetX - IMAGE_WIDTH / 2.0f * d_zoom;
        fi32_t startReal = offsetX  -  IMAGE_WIDTH/2 * zoom;

        for (int x = 0; x < IMAGE_WIDTH;  x++) {
          int value = max_iters - 1;

          /*if (d_startReal < -3.5f || d_startReal > 3.5f || d_startImag < -3.5f || d_startImag > 3.5f) {
            image.setPixel(x, y, colors->getColor(value));
            startReal = startReal + zoom;
            break;
          }*/

          //double d_zReal = d_startReal;
          fi32_t zReal = startReal;
          //double d_zImag = d_startImag;
          fi32_t zImag = startImag;

          for (unsigned int counter = 0; counter < max_iters; counter++) {
            //double d_r2 = d_zReal * d_zReal;
            fi64_t r2_64 = ((fi64_t)(zReal)*(fi64_t)(zReal));
            fi32_t r2 = ((r2_64 >> FI_32_25) < MAX_FI_32) ? (fi32_t)(r2_64 >> FI_32_25) : MAX_FI_32;
            //disp_cmp_real(d_r2, r2, FI_32_25);
            //usleep(100000);

            //double d_i2 = d_zImag * d_zImag;
            fi64_t i2_64 = ((fi64_t)(zImag)*(fi64_t)(zImag));
            fi32_t i2 = ((i2_64 >> FI_32_25) < MAX_FI_32) ? (fi32_t)(i2_64 >> FI_32_25) : MAX_FI_32;
            //disp_cmp_real(d_i2, i2, FI_32_25);

            //d_zImag = 2.0f * d_zReal * d_zImag + d_startImag;
            fi64_t z_real_z_imag_64 = (fi64_t)zReal*(fi64_t)zImag;
            fi32_t z_real_z_imag = (fi32_t)(z_real_z_imag_64 >> FI_32_25);
            zImag = (2 * z_real_z_imag  + startImag);
            //disp_cmp_real(d_zImag, zImag, FI_32_25);
            //d_zReal = d_r2 - d_i2 + d_startReal;
            zReal = (r2  -  i2  +  startReal);
            //disp_cmp_real(d_zReal, zReal, FI_32_25);

            if ( (r2 + i2) > (4 << FI_32_25)) {
            //if ( (d_r2 + d_i2) > 4) {
              value = counter;
              break;
            }
          }
          image.setPixel(x, y, colors->getColor(value));
          //d_startReal += d_zoom;
          startReal = startReal + zoom;
        }
      }
    }
  }


    virtual void updateImage(const long double d_zoom, const long double d_offsetX, const long double d_offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr) {
//#pragma omp parallel // on declare une section parallel
        {
//#pragma omp for // on fait du multicoeur
            for (int y = 0; y < IMAGE_HEIGHT; y++) {

                fi32_t offsetX = double_to_fi32(d_offsetX, FI_32_25);
                fi32_t offsetY = double_to_fi32(d_offsetY, FI_32_25);

                fi32_t zoom = double_to_fi32(d_zoom, FI_32_25);

                //double d_startImag = d_offsetY  -  IMAGE_HEIGHT / 2.0f * d_zoom  +  y * d_zoom;
                fi32_t startImag = offsetY  -  IMAGE_HEIGHT/2 * zoom  +  y * zoom;

                //double d_startReal = d_offsetX - IMAGE_WIDTH / 2.0f * d_zoom;
                fi32_t startReal = offsetX  -  IMAGE_WIDTH/2 * zoom;

                float* ptr_o = ptr + y * IMAGE_WIDTH;

                for (int x = 0; x < IMAGE_WIDTH;  x++) {
                    int value = max_iters - 1;

                    /*if (d_startReal < -3.5f || d_startReal > 3.5f || d_startImag < -3.5f || d_startImag > 3.5f) {
                      image.setPixel(x, y, colors->getColor(value));
                      startReal = startReal + zoom;
                      break;
                    }*/

                    //double d_zReal = d_startReal;
                    fi32_t zReal = startReal;
                    //double d_zImag = d_startImag;
                    fi32_t zImag = startImag;

                    for (unsigned int counter = 0; counter < max_iters; counter++) {
                        //double d_r2 = d_zReal * d_zReal;
                        fi64_t r2_64 = ((fi64_t)(zReal)*(fi64_t)(zReal));
                        fi32_t r2 = ((r2_64 >> FI_32_25) < MAX_FI_32) ? (fi32_t)(r2_64 >> FI_32_25) : MAX_FI_32;
                        //disp_cmp_real(d_r2, r2, FI_32_25);
                        //usleep(100000);

                        //double d_i2 = d_zImag * d_zImag;
                        fi64_t i2_64 = ((fi64_t)(zImag)*(fi64_t)(zImag));
                        fi32_t i2 = ((i2_64 >> FI_32_25) < MAX_FI_32) ? (fi32_t)(i2_64 >> FI_32_25) : MAX_FI_32;
                        //disp_cmp_real(d_i2, i2, FI_32_25);

                        //d_zImag = 2.0f * d_zReal * d_zImag + d_startImag;
                        fi64_t z_real_z_imag_64 = (fi64_t)zReal*(fi64_t)zImag;
                        fi32_t z_real_z_imag = (fi32_t)(z_real_z_imag_64 >> FI_32_25);
                        zImag = (2 * z_real_z_imag  + startImag);
                        //disp_cmp_real(d_zImag, zImag, FI_32_25);
                        //d_zReal = d_r2 - d_i2 + d_startReal;
                        zReal = (r2  -  i2  +  startReal);
                        //disp_cmp_real(d_zReal, zReal, FI_32_25);

                        if ( (r2 + i2) > (4 << FI_32_25)) {
                            //if ( (d_r2 + d_i2) > 4) {
                            value = counter;
                            break;
                        }
                    }

                    (*ptr_o++) = value;
                    startReal = startReal + zoom;
                }
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
