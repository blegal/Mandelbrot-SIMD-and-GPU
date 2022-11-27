#include <SFML/Graphics.hpp>
//#include <Mouse.hpp>
#include <array>
#include <vector>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <sstream>

#include "Utils/Utils.hpp"
#include "Utils/Settings.hpp"
#include "Utils/FileHandler.hpp"
#include "Mandelbrot.hpp"

#include "Utils/StringUtils.hpp"
#include <getopt.h>



int main(int argc, char* argv[])
{

    int32_t width      = 1200;
    int32_t height     = 800;
    int32_t iterations = 256;

    bool benchmark = false;

    // Apply parameters :
    long double offsetX = -0.70000000000000000; // and move around
    long double offsetY =  0.00000000000000000;
    long double zoom    =  0.00400000000000000;    // allow the user to zoom in and out...

    long double wheelZoomFactor = 0.05;


    static struct option long_options[] = {
        {"width",      required_argument,  0,  'w' },
        {"height",     required_argument,  0,  'h' },
        {"iters",      required_argument,  0,  'i' },
        {"benchmark",  no_argument,        0,  'b' },
        {0,                            0,  0,   0  }
    };
	while (1)
	{
        int option_index = 0;
		
		int c = getopt_long(argc, argv, "I:V:w:h:d:vs:f:", long_options, &option_index);
        if (c == -1)
            break;

    	switch (c) {
            case 0:
                printf("option %s", long_options[option_index].name);
                if (optarg)
                    printf(" with arg %s", optarg);
                printf("\n");
                break;

            case 'w': width      = atoi( optarg ); break;
            case 'h': height     = atoi( optarg ); break;
            case 'i': iterations = atoi( optarg ); break;

            case 'b':
                benchmark  = true;
                offsetX = -1.41718135711085513;
                offsetY = -0.00150879989408595;
                zoom    =  0.00004280112436037;
                break;

       default:
            printf("?? getopt returned character code 0%o ??\n",     c);
            printf("?? getopt returned character code %d ??\n", (int)c);
            printf("?? getopt returned character code %c ??\n",      c);
        }
    }


    printf("(II) Dimension de la fenetre (%d, %d)\n", width, height);

    Mandelbrot mb( width, height, iterations );

    if( benchmark == true )
    {
        width   = 3840;
        height  = 2160;
        offsetX = -0.71595446758947083;
        offsetY = -0.28415288527686088;
        zoom    = +0.00000775942804870;
        
        mb.RunBenchmark(zoom, offsetX, offsetY);
        
        exit( EXIT_FAILURE );
    }

    //
    // Window creation
    //
    sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot - Premium HD edition");
    window.setFramerateLimit(0);

    sf::Image image;
    image.create(width, height, sf::Color(0, 0, 0));
    sf::Texture texture;
    sf::Sprite sprite;

    bool stateChanged    = true; // track whether the image needs to be regenerated
    bool showElapsedTime = false; // track whether the image needs to be regenerated
//    sf::Clock clicTime;

    std::vector<unsigned int> execTimes;

    window.setFramerateLimit(20); // Limite à 60 images par seconde

    while (window.isOpen()) {

        sf::Event event;
        sf::Vector2i position;
        sf::Vector2<long double> worldPosition;
        sf::Vector2i distanceToCenter;

        while (window.pollEvent(event)) {

            switch (event.type) {

                case sf::Event::MouseWheelScrolled:
                    stateChanged = true;
                    position     = sf::Mouse::getPosition(window);

                    //Positionnement plus agréable :
                    worldPosition.x = (position.x - (long double)(width )/2.0) * zoom + offsetX;
                    worldPosition.y = (position.y - (long double)(height)/2.0) * zoom + offsetY;

                    if (event.mouseWheelScroll.delta > 0) {
                        for (int i = 0 ; i < event.mouseWheelScroll.delta ; i++) {
                            offsetX += (worldPosition.x - offsetX) * wheelZoomFactor;
                            offsetY += (worldPosition.y - offsetY) * wheelZoomFactor;
                            zoom    /= wheelZoomFactor+1;
                        }
                    } else {
                        for (int i = 0 ; i < -event.mouseWheelScroll.delta ; i++) {
                            offsetX += (offsetX - worldPosition.x) * wheelZoomFactor;
                            offsetY += (offsetY - worldPosition.y) * wheelZoomFactor;
                            zoom    *= wheelZoomFactor + 1;
                        }
                    }
                    break;

                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                            stateChanged = true;
                            position = sf::Mouse::getPosition(window);
                            offsetX += (position.x - width /2) * zoom;
                            offsetY += (position.y - height/2) * zoom;
                    }
                    break;

                case sf::Event::Closed:
                    window.close();
                    break;
/*
                case sf::Event::Resized:
                    parameters.SetWidth(event.size.width);
                    parameters.SetHeight(event.size.height);
                    image.create(event.size.width, event.size.height, sf::Color(0, 0, 0));
                    printf("(II) Dimension de la fenetre (%d, %d)\n", width, height);
                    break;
*/
                case sf::Event::KeyPressed:
                    stateChanged = true; // image needs to be recreated when the user changes zoom or offset
                    switch (event.key.code) {
                        case sf::Keyboard::Escape :
                            window.close();
                            break;

                        case sf::Keyboard::A :
                            zoom *= 0.75;
                            break;

                        case sf::Keyboard::Z :
                            zoom /= 0.75;
                            break;

                        case sf::Keyboard::O :
                            iterations = (iterations > 1) ? (iterations / 2) : iterations;
                            printf("(II) Reducing the number of decoding iterations to %d\n", iterations);
                            mb.Iterations( iterations );
                            mb.Update();
                            break;

                        case sf::Keyboard::P :
                            iterations = (iterations <= 65536) ? (2 * iterations) : iterations;
                            printf("(II) Increasing the number of decoding iterations to %d\n", iterations);
                            mb.Iterations( iterations );
                            mb.Update();
                            break;

                        case sf::Keyboard::I :
                            printf("(II) Informations : offsetX = %0.17Lf | offsetY = %0.17Lf | zoom = %0.17Lf |\n", offsetX, offsetY, zoom);
                            break;

                        case sf::Keyboard::C :
//                            parameters.SetCentralDot( !parameters.isCentralDotEnabled );
//                            break;

                        case sf::Keyboard::R :
                            break;

                        case sf::Keyboard::T :
                            showElapsedTime = !showElapsedTime;
                            break;

                        case sf::Keyboard::Up:
                            mb.previousConvergence();
                            break;

                        case sf::Keyboard::Down:
                            mb.nextConvergence();
                            break;

                        case sf::Keyboard::Left:
                            mb.previousColorMap();
                            break;

                        case sf::Keyboard::Right:
                            mb.nextColorMap();
                            break;

                        default:
                            stateChanged = false;
                            break;

                        case sf::Keyboard::B :
                            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
                            for(uint32_t q = 0; q < 64; q += 1)
                                mb.Benchmark(zoom, offsetX, offsetY);
                            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                            std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() <<std::endl;
                            break;
                    }
                default:
                    break;
            }
        }

#if 0
        if (parameters.autoZoom && !autoZoomFinished && zoom < parameters.finalZoom && parameters.nbSimulations > 0) {
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            //calcul stats
            unsigned int nbSimu = parameters.nbSimulations;
            unsigned int curentDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - autoZoomBegin).count();
            execTimes.push_back(curentDuration);
            sumDuration += curentDuration;
            maxDuration = (maxDuration > curentDuration ? maxDuration : curentDuration);
            minDuration = (minDuration < curentDuration ? minDuration : curentDuration);
            avgTime = sumDuration/compteur_autoZoom;
            medianTime = median(execTimes);
            if (nbSimu == 1) {
                std:: cout << "AutoZoom terminé, durée totale : " << curentDuration/1000.0f <<  " s" << std::endl;
            } else {
                std:: cout << "AutoZoom "<<compteur_autoZoom<<"/"<< nbSimu << " terminé, durée totale : " << curentDuration/1000.0f <<  " s" << std::endl;
            }
            //actualisation variables
            compteur_autoZoom += 1;
            if (compteur_autoZoom <= nbSimu) {
                zoom = parameters.zoom; //zoom remis a zero
                autoZoomBegin = std::chrono::steady_clock::now();//remise pour que les calculs de stats n'ai pas d'influence
            }
            if (compteur_autoZoom > nbSimu) {
                autoZoomFinished = true; //fini
                execTimes.clear();
                std::cout << "Temps total  : " << sumDuration/1000.0f << " s" <<std::endl;
                if (nbSimu > 1) {
                    std::cout << "Temps median : " << medianTime/1000.0f  << " s" <<std::endl;
                    std::cout << "Temps moyen  : " << avgTime/1000.0f     << " s" <<std::endl;
                    std::cout << "Temps max    : " << maxDuration/1000.0f << " s" <<std::endl;
                    std::cout << "Temps min    : " << minDuration/1000.0f << " s" <<std::endl;
                }

                if (parameters.logTofile) {
                    unsigned int c_off = parameters.GetConvOffset();
                    tab_csv[TAB_CSV_VSTART+c_off][TAB_CSV_HSTART+parameters.testID-1]                 = ToString(medianTime /1000.0f);
                    tab_csv[TAB_CSV_VSTART+c_off+TAB_CSV_VSTEP][TAB_CSV_HSTART+parameters.testID-1]   = ToString(avgTime    /1000.0f);
                    tab_csv[TAB_CSV_VSTART+c_off+TAB_CSV_VSTEP*2][TAB_CSV_HSTART+parameters.testID-1] = ToString(maxDuration/1000.0f);
                    tab_csv[TAB_CSV_VSTART+c_off+TAB_CSV_VSTEP*3][TAB_CSV_HSTART+parameters.testID-1] = ToString(minDuration/1000.0f);

                    //CSVHandler::PrintCSV(tab_csv);
                    std::cout << "Sauvegarde des resultats..." << std::endl;
                    bool success;
                    if (parameters.lastSimulation || parameters.testID == 0) {
                        tab_csv[2][12] = getOSname();
                        tab_csv[3][12] = getCPUname();
                        success = CSVHandler::WriteCSV(tab_csv, "results_" + dateTimeString() + ".csv", parameters);
                        remove("tmp.csv");
                    } else {
                        success = CSVHandler::WriteCSV(tab_csv, "tmp.csv", parameters);
                    }
                    if (success) std::cout << "Sauvegarde reussie !" << std::endl;
                }
                if (parameters.closeAfterSimulation) {
                    window.close();
                }
            } else {
                autoZoomFinished = false; //pas fini
            }
        }
#endif

        if (stateChanged) {
            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            mb.updateImage(zoom, offsetX, offsetY, image);
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            
            if ( showElapsedTime == true )
            {
                std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() <<std::endl;
            }
            texture.loadFromImage(image);
            sprite.setTexture(texture);
            stateChanged = false;
        }
        window.draw(sprite);

        window.display();
    }
}
