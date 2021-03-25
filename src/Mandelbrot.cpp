#include "Mandelbrot.hpp"

Mandelbrot::Mandelbrot(Settings* p) {
    params    = p;
    c         = nullptr;
    colors    = nullptr;

    allocRessources( );

    const uint32_t nPixels = params->Width() * params->Height();

    f_witdh  = params->Width();
    f_height = params->Height();

    iter_icount = new uint32_t[nPixels];
    iter_fcount = new float   [nPixels];

}


Mandelbrot::~Mandelbrot()
{
    freeRessources( );
    delete[] iter_icount;
    delete[] iter_fcount;
}


void Mandelbrot::allocRessources( ){

    if( colors == nullptr )
        colors = library.get( params->ColorMapMode() );
    else
        colors = library.get();
    colors->setIters( params->Iterations() );

    if( c == nullptr )
        c = converge.get( params->ConvergenceType() );
    else
        c = converge.get();

    c->setIters( params->Iterations() );
    c->setColor( colors );
}


void Mandelbrot::freeRessources( )
{
//    delete c;
//    delete colors;
}


//
//
//
void Mandelbrot::Update(){
    freeRessources( );
    allocRessources( );
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

void Mandelbrot::listAllModules()
{
    converge.listAllModules();
}



void Mandelbrot::Benchmark(const long double zoom, const long double offsetX, const long double offsetY)
{
    assert( f_witdh  == params->Width() );
    assert( f_height == params->Height() );
    c->updateImage(zoom, offsetX, offsetY, params->Width(), params->Height(), iter_fcount);
}

//
//
//
void Mandelbrot::updateImage(const long double zoom, const long double offsetX, const long double offsetY, sf::Image& image)
{
    assert( f_witdh  == params->Width() );
    assert( f_height == params->Height() );

    //printf("(II) Mandelbrot::updateImage : offsetX = %0.17Lf | offsetY = %0.17Lf | zoom = %0.17Lf |\n", offsetX, offsetY, zoom);
#if 0
    c->updateImage(zoom, offsetX, offsetY, params->Width(), params->Height(), image);
#else
    c->updateImage(zoom, offsetX, offsetY, params->Width(), params->Height(), iter_fcount);
    colors->colorize(params->Width(), params->Height(), iter_fcount, image);
#endif
    //
    //  Affichage du marqueur central si l'utilisateur l'a souhaité
    //

    if (params->isCentralDotEnabled) {
        sf::Color white(255, 255, 255);
        image.setPixel(params->Width()/2-1, params->Height()/2,   white);
        image.setPixel(params->Width()/2+1, params->Height()/2,   white);
        image.setPixel(params->Width()/2,   params->Height()/2,   white);
        image.setPixel(params->Width()/2,   params->Height()/2-1, white);
        image.setPixel(params->Width()/2,   params->Height()/2+1, white);
    }
}
