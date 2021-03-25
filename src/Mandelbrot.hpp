#ifndef MANDELBROT_H_
#define MANDELBROT_H_

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Utils/Settings.hpp"
#include "Convergence/Convergence.hpp"
#include "Color/library/ColorLibrary.hpp"
#include "Convergence/library/ConvergenceLibrary.hpp"

enum t_format    { f_double, f_float, f_int };
enum t_processor { x86, sse, avx, cuda };

class Mandelbrot {
private:
    ColorMap* colors;
    Convergence* c;

    uint32_t IMAGE_WIDTH;
    uint32_t IMAGE_HEIGHT;
//    unsigned int max_iters;


public:
    Mandelbrot(Settings* p);

    ~Mandelbrot();

    void freeRessources();

    void allocRessources();

    void Update();

    void Benchmark(const long double zoom, const long double offsetX, const long double offsetY);
    void updateImage(const long double zoom, const long double offsetX, const long double offsetY, sf::Image& image);

    void nextColorMap();
    void previousColorMap();
    void nextConvergence();
    void previousConvergence();

    void listAllModules();

private:
    int MAX;
    void updateImageSlice(const long double zoom, const long double offsetX, const long double offsetY, sf::Image& image, int minY, int maxY);
    ColorLibrary       library;
    ConvergenceLibrary converge;
    Settings*          params;

    uint32_t* iter_icount;
    float*    iter_fcount;

    uint32_t f_witdh;
    uint32_t f_height;

};


#endif
