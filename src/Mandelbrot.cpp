#include "Mandelbrot.hpp"
#include <chrono>

Mandelbrot::Mandelbrot(const int32_t Width, const int32_t Height, const int32_t iterations) {
    c         = nullptr;
    colors    = nullptr;

    f_width    = Width;
    f_height   = Height;
    iters      = iterations;

    const uint32_t nPixels = Width * Height;
    iter_icount = new uint32_t[nPixels];
    iter_fcount = new float   [nPixels];

    allocRessources( );
}


Mandelbrot::~Mandelbrot()
{
    freeRessources( );
    delete[] iter_icount;
    delete[] iter_fcount;
}


void Mandelbrot::allocRessources( ){

//    if( colors == nullptr )
//        colors = library.get( params->ColorMapMode() );
//    else
    colors = library.get();
    colors->setIters( iters );

//    if( c == nullptr )
//        c = converge.get( params->ConvergenceType() );
//    else
    c = converge.get();
    c->setIters( iters  );
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


void Mandelbrot::Iterations(const int32_t iterations)
{
    iters = iterations;
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
    assert( f_witdh  == params->Width()  );
    assert( f_height == params->Height() );
    c->updateImage(zoom, offsetX, offsetY, f_width, f_height, iter_fcount);
}


void Mandelbrot::RunBenchmark(const long double zoom, const long double offsetX, const long double offsetY)
{
    std::cout << "(II) Lancement du benchmark ("  << converge.size() << ")"  << std::endl;
    for( int32_t m = 0; m < converge.size(); m += 1 )
    {
        Convergence* cc = converge.list.at( m );
        auto start = chrono::steady_clock::now();

        const int32_t nTests = 2;
        for( int32_t loop = 0; loop < nTests; loop += 1 )
        {
            cc->updateImage(zoom, offsetX, offsetY, f_width, f_height, iter_fcount);
        }
        auto end = chrono::steady_clock::now();

        std::string item  = cc->toShortString();
        const auto mtime = chrono::duration_cast<chrono::microseconds>(end - start).count();
        cout << "|" << item;
        printf("%8ld us|\n", (mtime/ nTests));

    }
//    c->updateImage(zoom, offsetX, offsetY, f_width, f_height, iter_fcount);

    exit(EXIT_SUCCESS);
}


//
//
//
void Mandelbrot::updateImage(const long double zoom, const long double offsetX, const long double offsetY, sf::Image& image)
{
    assert( f_witdh  == params->Width() );
    assert( f_height == params->Height() );

    c->updateImage(zoom, offsetX, offsetY, f_width, f_height, iter_fcount);
    colors->colorize(f_width, f_height, iter_fcount, image);

    //
    //  Affichage du marqueur central si l'utilisateur l'a souhaité
    //

    if ( false /*params->isCentralDotEnabled*/)
    {
        sf::Color white(255, 255, 255);
        image.setPixel(f_width/2-1, f_height/2,   white);
        image.setPixel(f_width/2+1, f_height/2,   white);
        image.setPixel(f_width/2,   f_height/2,   white);
        image.setPixel(f_width/2,   f_height/2-1, white);
        image.setPixel(f_width/2,   f_height/2+1, white);
    }
}
