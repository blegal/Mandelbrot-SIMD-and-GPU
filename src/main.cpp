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
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

#include "Utils/Image.hpp"
#include "Mandelbrot.hpp"

#if defined(_ENABLE_GUI_)
    #include "Display/Window.hpp"
#endif


//
// Vue initiale (coin superieur gauche = offset - taille/2 * zoom)
//
static const long double DEFAULT_OFFSET_X = -0.70000000000000000;
static const long double DEFAULT_OFFSET_Y =  0.00000000000000000;
static const long double DEFAULT_ZOOM     =  0.00400000000000000;

//
// Le tableau des palettes contient 65536 couleurs (cf. ColorMap.hpp)
//
static const int32_t MAX_ITERATIONS = 32768;


static void usage(const char* program)
{
    printf("Usage: %s [options]\n", program);
    printf("\n");
    printf("Display options:\n");
    printf("  -w, --width  <px>          window / image width              (default: 1200)\n");
    printf("  -h, --height <px>          window / image height             (default: 800)\n");
    printf("  -i, --iters  <n>           maximum number of iterations      (default: 256, max: %d)\n", MAX_ITERATIONS);
    printf("  -e, --engine <name|id>     computation engine (see --list)   (default: first one)\n");
    printf("  -c, --colormap <name|id>   color palette (see --list)        (default: first one)\n");
    printf("  -x, --offset-x <real>      real part of the image center     (default: -0.7)\n");
    printf("  -y, --offset-y <real>      imaginary part of the image center (default: 0.0)\n");
    printf("  -z, --zoom <real>          size of one pixel in the complex plane (default: 0.004)\n");
    printf("  -d, --dot                  draw a marker at the center of the image\n");
    printf("\n");
    printf("Batch options:\n");
    printf("  -o, --output <file>        render a single image (.png, .ppm, .bmp) and exit\n");
    printf("  -b, --benchmark            benchmark every available engine and exit\n");
    printf("  -n, --runs <n>             number of runs per engine in benchmark mode (default: 2)\n");
    printf("  -l, --list                 list the available engines and color palettes\n");
    printf("      --help                 display this help\n");
}


#if defined(_ENABLE_GUI_)
static void keyboard_help()
{
    printf("(II) Keyboard and mouse bindings:\n");
    printf("     mouse wheel     : zoom in / out around the cursor\n");
    printf("     left click      : center the view on the cursor\n");
    printf("     A / Z           : zoom in / zoom out\n");
    printf("     O / P           : halve / double the number of iterations\n");
    printf("     Up / Down       : previous / next computation engine\n");
    printf("     Left / Right    : previous / next color palette\n");
    printf("     R               : reset the view\n");
    printf("     C               : toggle the central marker\n");
    printf("     I               : print the current coordinates\n");
    printf("     T               : toggle the display of the rendering time\n");
    printf("     B               : benchmark the current engine (64 frames)\n");
    printf("     S               : save a screenshot (PNG)\n");
    printf("     H               : display this help\n");
    printf("     Escape          : quit\n");
}


static std::string screenshot_name()
{
    char buffer[64];
    const time_t now = time( nullptr );
    strftime(buffer, sizeof(buffer), "mandelbrot-%Y%m%d-%H%M%S.png", localtime( &now ));
    return buffer;
}
#endif


int main(int argc, char* argv[])
{
    int32_t width      = 1200;
    int32_t height     = 800;
    int32_t iterations = 256;
    int32_t runs       = 2;

    bool benchmark  = false;
    bool list       = false;
    bool centralDot = false;
    bool userSize   = false;
    bool userView   = false;

    std::string engine;
    std::string colormap;
    std::string output;

    // Apply parameters :
    long double offsetX = DEFAULT_OFFSET_X; // and move around
    long double offsetY = DEFAULT_OFFSET_Y;
    long double zoom    = DEFAULT_ZOOM;     // allow the user to zoom in and out...

    long double wheelZoomFactor = 0.05;

    //
    // Analyse de la ligne de commande (sans getopt, pour rester portable)
    //
    for (int a = 1; a < argc; a += 1)
    {
        const std::string opt = argv[a];

        auto value = [&]() -> const char* {
            if (a + 1 >= argc) {
                fprintf(stderr, "(EE) Option %s requires an argument\n", opt.c_str());
                exit( EXIT_FAILURE );
            }
            return argv[++a];
        };

        if      ( opt == "-w" || opt == "--width"     ) { width      = atoi  ( value() ); userSize = true; }
        else if ( opt == "-h" || opt == "--height"    ) { height     = atoi  ( value() ); userSize = true; }
        else if ( opt == "-i" || opt == "--iters"     ) { iterations = atoi  ( value() ); }
        else if ( opt == "-x" || opt == "--offset-x"  ) { offsetX    = strtold( value(), nullptr ); userView = true; }
        else if ( opt == "-y" || opt == "--offset-y"  ) { offsetY    = strtold( value(), nullptr ); userView = true; }
        else if ( opt == "-z" || opt == "--zoom"      ) { zoom       = strtold( value(), nullptr ); userView = true; }
        else if ( opt == "-e" || opt == "--engine"    ) { engine     = value(); }
        else if ( opt == "-c" || opt == "--colormap"  ) { colormap   = value(); }
        else if ( opt == "-o" || opt == "--output"    ) { output     = value(); }
        else if ( opt == "-n" || opt == "--runs"      ) { runs       = atoi  ( value() ); }
        else if ( opt == "-b" || opt == "--benchmark" ) { benchmark  = true; }
        else if ( opt == "-l" || opt == "--list"      ) { list       = true; }
        else if ( opt == "-d" || opt == "--dot"       ) { centralDot = true; }
        else if ( opt == "-?" || opt == "--help"      ) { usage( argv[0] ); return EXIT_SUCCESS; }
        else
        {
            fprintf(stderr, "(EE) Unknown option : %s\n\n", opt.c_str());
            usage( argv[0] );
            return EXIT_FAILURE;
        }
    }

    if( (width <= 0) || (height <= 0) || (iterations <= 1) || (iterations > MAX_ITERATIONS) || (runs <= 0) || (zoom <= 0) )
    {
        fprintf(stderr, "(EE) Invalid parameter value (see --help)\n");
        return EXIT_FAILURE;
    }

    if( benchmark == true )
    {
        printf("(II) Activating benchmarking mode\n");
        if( userSize == false ) {
            width   = 3840;
            height  = 2160;
        }
        if( userView == false ) {
            offsetX = -0.71595446758947083;
            offsetY = -0.28415288527686088;
            zoom    = +0.00000775942804870;
        }
    }

    printf("(II) Dimension de la fenetre (%d, %d)\n", width, height);

    Mandelbrot mb( width, height, iterations );
    mb.CentralDot( centralDot );

    if( (engine.empty() == false) && (mb.selectConvergence( engine ) == false) )
    {
        fprintf(stderr, "(EE) Unknown computation engine : %s (see --list)\n", engine.c_str());
        return EXIT_FAILURE;
    }

    if( (colormap.empty() == false) && (mb.selectColorMap( colormap ) == false) )
    {
        fprintf(stderr, "(EE) Unknown color palette : %s (see --list)\n", colormap.c_str());
        return EXIT_FAILURE;
    }

    if( list == true )
    {
        printf("AVAILABLE ENGINES:\n");
        mb.listAllModules();
        printf("AVAILABLE COLOR PALETTES:\n");
        mb.listAllColorMaps();
        return EXIT_SUCCESS;
    }

    if( benchmark == true )
    {
        mb.RunBenchmark(zoom, offsetX, offsetY, runs);
        return EXIT_SUCCESS;
    }

    gfx::Image image;
    image.create(width, height, gfx::Color(0, 0, 0));

    if( output.empty() == false )
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        mb.updateImage(zoom, offsetX, offsetY, image);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "(II) Rendering time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;

        if( image.saveToFile( output ) == false )
        {
            fprintf(stderr, "(EE) Unable to write the image file : %s\n", output.c_str());
            return EXIT_FAILURE;
        }
        printf("(II) Image saved to %s\n", output.c_str());
        return EXIT_SUCCESS;
    }

#if !defined(_ENABLE_GUI_)
    fprintf(stderr, "(EE) This binary was built without the graphical interface (MANDELBROT_ENABLE_GUI=OFF).\n");
    fprintf(stderr, "(EE) Use --output <file.png>, --benchmark or --list.\n");
    return EXIT_FAILURE;
#else

    //
    // Window creation
    //
    gfx::Window window(width, height, "Mandelbrot - Premium HD edition");

    keyboard_help();

    bool stateChanged    = true;  // track whether the image needs to be regenerated
    bool showElapsedTime = false; // print the rendering time of each frame

    while (window.isOpen()) {

        gfx::Event event;
        long double worldX, worldY;

        window.waitEvents( 0.1 );

        while (window.pollEvent(event)) {

            switch (event.type) {

                case gfx::Event::MouseWheelScrolled:
                {
                    stateChanged = true;

                    //Positionnement plus agréable : le point sous le curseur reste fixe
                    worldX = (event.x - (long double)(width )/2.0) * zoom + offsetX;
                    worldY = (event.y - (long double)(height)/2.0) * zoom + offsetY;

                    const long double ratio = powl(wheelZoomFactor + 1, -event.delta);
                    offsetX = worldX - (worldX - offsetX) * ratio;
                    offsetY = worldY - (worldY - offsetY) * ratio;
                    zoom   *= ratio;
                    break;
                }

                case gfx::Event::MouseButtonPressed:
                    if (event.button == gfx::MouseButton::Left) {
                            stateChanged = true;
                            offsetX += (event.x - width /2) * zoom;
                            offsetY += (event.y - height/2) * zoom;
                    }
                    break;

                case gfx::Event::Closed:
                    window.close();
                    break;

                case gfx::Event::KeyPressed:
                    stateChanged = true; // image needs to be recreated when the user changes zoom or offset
                    switch (event.key) {
                        case gfx::Key::Escape :
                            window.close();
                            break;

                        case gfx::Key::A :
                            zoom *= 0.75;
                            break;

                        case gfx::Key::Z :
                            zoom /= 0.75;
                            break;

                        case gfx::Key::O :
                            iterations = (iterations > 2) ? (iterations / 2) : iterations;
                            printf("(II) Reducing the number of decoding iterations to %d\n", iterations);
                            mb.Iterations( iterations );
                            mb.Update();
                            break;

                        case gfx::Key::P :
                            iterations = (iterations <= MAX_ITERATIONS / 2) ? (2 * iterations) : iterations;
                            printf("(II) Increasing the number of decoding iterations to %d\n", iterations);
                            mb.Iterations( iterations );
                            mb.Update();
                            break;

                        case gfx::Key::I :
                            printf("(II) Informations : offsetX = %0.17Lf | offsetY = %0.17Lf | zoom = %0.17Lg |\n", offsetX, offsetY, zoom);
                            printf("(II) Command line : -x %0.17Lf -y %0.17Lf -z %0.17Lg -i %d -e %s -c %s\n", offsetX, offsetY, zoom, iterations, mb.convergenceName().c_str(), mb.colorMapName().c_str());
                            break;

                        case gfx::Key::C :
                            mb.CentralDot( !mb.CentralDot() );
                            break;

                        case gfx::Key::R :
                            offsetX = DEFAULT_OFFSET_X;
                            offsetY = DEFAULT_OFFSET_Y;
                            zoom    = DEFAULT_ZOOM;
                            break;

                        case gfx::Key::T :
                            showElapsedTime = !showElapsedTime;
                            break;

                        case gfx::Key::H :
                            keyboard_help();
                            break;

                        case gfx::Key::S :
                        {
                            const std::string name = screenshot_name();
                            if( image.saveToFile( name ) )
                                printf("(II) Screenshot saved to %s\n", name.c_str());
                            break;
                        }

                        case gfx::Key::Up:
                            mb.previousConvergence();
                            break;

                        case gfx::Key::Down:
                            mb.nextConvergence();
                            break;

                        case gfx::Key::Left:
                            mb.previousColorMap();
                            break;

                        case gfx::Key::Right:
                            mb.nextColorMap();
                            break;

                        case gfx::Key::B :
                        {
                            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
                            for(uint32_t q = 0; q < 64; q += 1)
                                mb.Benchmark(zoom, offsetX, offsetY);
                            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                            std::cout << "(II) 64 frames rendered in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;
                            break;
                        }

                        default:
                            stateChanged = false;
                            break;
                    }
                    break;

                default:
                    break;
            }
        }

        if (stateChanged) {
            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            mb.updateImage(zoom, offsetX, offsetY, image);
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

            const long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
            if ( showElapsedTime == true )
            {
                std::cout << "Time difference = " << elapsed << std::endl;
            }

            char title[256];
            snprintf(title, sizeof(title), "Mandelbrot - %s - %s - %d iterations - %lld ms",
                     mb.convergenceName().c_str(), mb.colorMapName().c_str(), iterations, elapsed);
            window.setTitle( title );

            window.draw( image );
            stateChanged = false;
        }

        window.display();
    }

    return EXIT_SUCCESS;
#endif
}
