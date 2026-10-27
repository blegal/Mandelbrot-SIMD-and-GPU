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
#include "Mandelbrot.hpp"
#include <chrono>

Mandelbrot::Mandelbrot(const int32_t Width, const int32_t Height, const int32_t iterations) {
    c           = nullptr;
    colors      = nullptr;

    f_width     = Width;
    f_height    = Height;
    iters       = iterations;
    central_dot = false;

    const uint32_t nPixels = Width * Height;
    iter_fcount = new float[nPixels];

    allocRessources( );
}


Mandelbrot::~Mandelbrot()
{
    freeRessources( );
    delete[] iter_fcount;
}


//
// Les palettes et les moteurs appartiennent aux bibliotheques (library et
// converge) : on se contente ici de recuperer l'element courant.
//
void Mandelbrot::allocRessources( ){
    colors = library.get();
    colors->setIters( iters );

    c = converge.get();
    c->setIters( iters  );
    c->setColor( colors );
}


void Mandelbrot::freeRessources( )
{
    colors = nullptr;
    c      = nullptr;
}


//
//
//
void Mandelbrot::Update(){
    freeRessources( );
    allocRessources( );
}


void Mandelbrot::Iterations(const int32_t iterations)
{
    iters = iterations;
}


void Mandelbrot::CentralDot(const bool enable)
{
    central_dot = enable;
}


bool Mandelbrot::CentralDot()
{
    return central_dot;
}


//
//
//
void Mandelbrot::nextColorMap(){
    freeRessources ();
    library.next   ();
    allocRessources();
}


//
//
//
void Mandelbrot::previousColorMap(){
    freeRessources  ();
    library.previous();
    allocRessources ();
}


//
//
//
void Mandelbrot::nextConvergence(){
    freeRessources  ();
    converge.next();
    allocRessources ();
}


//
//
//
void Mandelbrot::previousConvergence(){
    freeRessources  ();
    converge.previous();
    allocRessources ();
}


//
//
//
bool Mandelbrot::selectColorMap(const std::string name)
{
    const int32_t num = library.find( name );
    if( num < 0 )
        return false;
    freeRessources ();
    library.get    ( num );
    allocRessources();
    return true;
}


//
//
//
bool Mandelbrot::selectConvergence(const std::string name)
{
    const int32_t num = converge.find( name );
    if( num < 0 )
        return false;
    freeRessources ();
    converge.get   ( num );
    allocRessources();
    return true;
}


std::string Mandelbrot::colorMapName()
{
    return colors->name();
}


std::string Mandelbrot::convergenceName()
{
    return c->name();
}


void Mandelbrot::listAllModules()
{
    converge.listAllModules();
}


void Mandelbrot::listAllColorMaps()
{
    library.listAllModules();
}


void Mandelbrot::Benchmark(const long double zoom, const long double offsetX, const long double offsetY)
{
    c->updateImage(zoom, offsetX, offsetY, f_width, f_height, iter_fcount);
}


void Mandelbrot::RunBenchmark(const long double zoom, const long double offsetX, const long double offsetY, const int32_t nTests)
{
    std::cout << "(II) Lancement du benchmark ("  << converge.size() << " moteurs, " << f_width << "x" << f_height << ", " << iters << " iterations)" << std::endl;
    for( int32_t m = 0; m < converge.size(); m += 1 )
    {
        Convergence* cc = converge.list.at( m );
        cc->setIters( iters );

        cc->updateImage(zoom, offsetX, offsetY, f_width, f_height, iter_fcount); // warm-up (allocations GPU, compilation des kernels, ...)

        auto start = std::chrono::steady_clock::now();
        for( int32_t loop = 0; loop < nTests; loop += 1 )
        {
            cc->updateImage(zoom, offsetX, offsetY, f_width, f_height, iter_fcount);
        }
        auto end = std::chrono::steady_clock::now();

        std::string item  = cc->toShortString();
        const long long mtime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "|" << item;
        printf("%10lld us|\n", (mtime / nTests));
    }
}


//
//
//
void Mandelbrot::updateImage(const long double zoom, const long double offsetX, const long double offsetY, gfx::Image& image)
{
    c->updateImage(zoom, offsetX, offsetY, f_width, f_height, iter_fcount);
    colors->colorize(f_width, f_height, iter_fcount, image);

    //
    //  Affichage du marqueur central si l'utilisateur l'a souhaité
    //

    if ( central_dot == true )
    {
        gfx::Color white(255, 255, 255);
        image.setPixel(f_width/2-1, f_height/2,   white);
        image.setPixel(f_width/2+1, f_height/2,   white);
        image.setPixel(f_width/2,   f_height/2,   white);
        image.setPixel(f_width/2,   f_height/2-1, white);
        image.setPixel(f_width/2,   f_height/2+1, white);
    }
}
