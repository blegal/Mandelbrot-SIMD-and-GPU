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
#include "CuteMode.hpp"
#include "Convergence/fixed/fixed_point.hpp"

#include "Utils/StringUtils.hpp"
#include <getopt.h>

#define LONG_OPTION 0
#define TAB_CSV_HSTART 2
#define TAB_CSV_VSTART 10
#define TAB_CSV_VSTEP 30


int main(int argc, char* argv[]) {

    printf("sizeof(        int) = %lu\n", sizeof(int));
    printf("sizeof(       long) = %lu\n", sizeof(long));
    printf("sizeof(  long long) = %lu\n", sizeof(long long));
    printf("sizeof(      float) = %lu\n", sizeof(float));
    printf("sizeof(     double) = %lu\n", sizeof(double));
    printf("sizeof(long double) = %lu\n", sizeof(long double));

    //
    // Parse parameters file :
    //
    string filename = "default_config.ini";
    if (argc > 1) {
        string argv1 = argv[1];
        if (argv1.at(0) != '-') {
            filename = argv[1];
        }
    }


    Settings     parameters;
    ConfigReader configFile(filename);
    configFile.ParseParams(parameters);

    // Parse command options :
    int opt = 0;
    int option_index = 0;

    static struct option long_options[] = {

            {"dp",           no_argument, 0, 0},
            {"dp_omp",       no_argument, 0, 0},
            {"dp_omp_avx",   no_argument, 0, 0},
            {"dp_omp_avx+",  no_argument, 0, 0},
            {"dp_omp_avx++", no_argument, 0, 0},

            {"sp",           no_argument, 0, 0},
            {"sp_omp",       no_argument, 0, 0},
            {"sp_omp_avx",   no_argument, 0, 0},
            {"sp_omp_avx+",  no_argument, 0, 0},
            //{"sp_omp_avx++", no_argument, 0, 0},

            {"fp",           no_argument, 0, 0},
            {"fp_omp",       no_argument, 0, 0},
            {"fp_omp_sse2",  no_argument, 0, 0},
            //{"fp_omp_avx2",  no_argument, 0, 0},

            {"close",        no_argument, 0, 0},
            {"cute",         no_argument, 0, 0},
            {"first",        no_argument, 0, 0},
            {"last",         no_argument, 0, 0},
            {"save",         no_argument, 0, 0},

            {"nbsimu",       required_argument, 0, 0},
            {"nbcudath",     required_argument, 0, 0},
            {"maxiter",      required_argument, 0, 0},
            {"testid",       required_argument, 0, 0},
            {"testID",       required_argument, 0, 0},

            {0, 0, 0, 0}
    };


    while ((opt = getopt_long(argc, argv, "x:y:w:h:i:d:csfl", long_options, &option_index)) != -1) {
        string long_opt;
        switch (opt) {
            case LONG_OPTION :
                long_opt = long_options[option_index].name;
                stringToUppercase(&long_opt);
                if (optarg) {
                    if (long_opt == "NBSIMU") {
                        parameters.SetNbSimulations(atoi(optarg));
                    }else if (long_opt == "NBCUDATH") {
                        parameters.SetNbCudaThreads(atoi(optarg));
                    } else if (long_opt == "MAXITER") {
                        parameters.Iterations(atoi(optarg));
                    } else if (long_opt == "MAXITER") {
                        parameters.Iterations(atoi(optarg));
                    } else if (long_opt == "TESTID") {
                        parameters.SetTestID(stringToInt(optarg));
                    }
                } else {
                    if (long_opt == "CLOSE") {
                        parameters.SetCloseAfterSimulation(true);
                    } else if (long_opt == "CUTE") {
                        parameters.SetCuteMode(true);
                    } else if (long_opt == "FIRST") {
                        parameters.SetFirstSimulation(true);
                    } else if (long_opt == "LAST") {
                        parameters.SetLastSimulation(true);
                    } else if (long_opt == "SAVE") {
                        parameters.SetLogToFile(true);
                    } else {
                        bool sucessfullyParsed = ConfigReader::ParseConvergenceType(long_opt, parameters);
                        if (!sucessfullyParsed) {
                            cout << "\033[33m" << endl; //unix only
                            cerr << "warning: unknown long option \"" << long_opt << "\""<< endl;
                            cout << "\033[0m"  << endl;
                            exit(-1);
                        }
                    }
                }
                break;

            case 'x' :
                parameters.SetOffsetX(stringToDouble(optarg));
                break;

            case 'y' :
                parameters.SetOffsetY(stringToDouble(optarg));
                break;

            case 'w' :
                parameters.Width(stringToInt(optarg));
                break;

            case 'h' :
                parameters.Height(stringToInt(optarg));
                break;

            case 'i' :
                parameters.Iterations(stringToInt(optarg));
                break;

            case 'c' : // close after simulation
                parameters.SetCloseAfterSimulation(true);
                break;

            case 'f' :
                parameters.SetFirstSimulation(true);
                break;

            case 'l' :
                parameters.SetLastSimulation(true);
                break;

            case 's' :
                parameters.SetLogToFile(true);
                break;

            case 'd' :
                parameters.SetTestID(stringToInt(optarg));
                break;

            default:
                /*cout << "\033[33m" << endl; //unix only
                cerr << "warning: invalid option received\"-" << opt << "\""<< endl;
                cout << "\033[0m"  << endl;*/
                exit(-1);
                break;
        }
    }

    //
    // Get result template
    //
    std::vector< std::vector< std::string > > tab_csv;
    if (parameters.firstSimulation || parameters.testID == 0) {
        tab_csv = CSVHandler::ReadCSV("results_template.csv", parameters);
    } else {
        tab_csv = CSVHandler::ReadCSV("tmp.csv", parameters);
    }

    //Kittens :
    CuteMode::LoadCuteness();

    // Apply parameters :
    long double offsetX = parameters.offsetX; // and move around
    long double offsetY = parameters.offsetY;
    long double zoom    = parameters.zoom;    // allow the user to zoom in and out...
//    int max_iters       = parameters.Iterations();

    unsigned short iterations; //256;
    long double wheelZoomFactor = 0.05;

    printf("(II) Dimension de la fenetre (%d, %d)\n", parameters.Width(), parameters.Height());

    // Mandelbrot object instanciation
    Mandelbrot mb( &parameters );
//    mb.listAllModules();

    // Window creation
    sf::RenderWindow window(sf::VideoMode(parameters.Width(), parameters.Height()), "Mandelbrot - Premium HD edition");
    window.setFramerateLimit(0);

    sf::Image image;
    image.create(parameters.Width(), parameters.Height(), sf::Color(0, 0, 0));
    sf::Texture texture;
    sf::Sprite sprite;

    bool stateChanged    = true; // track whether the image needs to be regenerated
    bool showElapsedTime = false; // track whether the image needs to be regenerated
//    sf::Clock clicTime;

    sf::Clock autoZoomTime;
//    std::chrono::steady_clock::time_point autoZoomBegin = std::chrono::steady_clock::now();
//    unsigned int sumDuration = 0;
//    unsigned int avgTime     = 0;
//    unsigned int medianTime  = 0;
//    unsigned int maxDuration = 0;
//    unsigned int minDuration = 1000000;

    bool autoZoomFinished          = false;
//    bool autoZoomFinished_10times  = false;
//    unsigned int compteur_autoZoom = 1;
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
                    worldPosition.x = (position.x - (long double)(parameters.Width() )/2.0) * zoom + offsetX;
                    worldPosition.y = (position.y - (long double)(parameters.Height())/2.0) * zoom + offsetY;

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
                            offsetX += (position.x - parameters.Width() /2) * zoom;
                            offsetY += (position.y - parameters.Height()/2) * zoom;
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
                    printf("(II) Dimension de la fenetre (%d, %d)\n", parameters.Width(), parameters.Height());
                    break;
*/
                case sf::Event::KeyPressed:
                    stateChanged = true; // image needs to be recreated when the user changes zoom or offset
                    switch (event.key.code) {
                        case sf::Keyboard::Escape :
                            window.close();
                            break;

                        case sf::Keyboard::F12 :
                            image.saveToFile("img/Mandelbrot_" + dateTimeString() + ".png");
                            break;

                        case sf::Keyboard::A :
                            zoom *= 0.75;
                            break;

                        case sf::Keyboard::Z :
                            zoom /= 0.75;
                            break;

                        case sf::Keyboard::O :
                            iterations = parameters.Iterations() / 2;
                            iterations = (parameters.Iterations() == 1) ? 1 : iterations;
                            printf("(II) Reducing the number of decoding iterations from %d to %d\n", parameters.Iterations(), iterations);
                            parameters.Iterations( iterations );
                            mb.Update();
                            break;

                        case sf::Keyboard::P :
                            iterations = parameters.Iterations() * 2;
                            iterations = (iterations <= 65535) ? iterations : 65535;
                            printf("(II) Increasing the number of decoding iterations from %d to %d\n", parameters.Iterations(), iterations);
                            parameters.Iterations( iterations );
                            mb.Update();
                            break;

                        case sf::Keyboard::I :
                            printf("(II) Informations : offsetX = %0.17Lf | offsetY = %0.17Lf | zoom = %0.17Lf |\n", offsetX, offsetY, zoom);
                            break;

                        case sf::Keyboard::C :
                            parameters.SetCentralDot( !parameters.isCentralDotEnabled );
                            break;

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

        if (parameters.autoZoom && parameters.nbSimulations > 0 && !autoZoomFinished && autoZoomTime.getElapsedTime() > sf::seconds(parameters.zoomStepTime)) {
            zoom /= parameters.zoomFactor;
            autoZoomTime.restart();
            stateChanged = true;
        }


        if (stateChanged) {
            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            mb.updateImage(zoom, offsetX, offsetY, image);
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            
            if ( (!parameters.autoZoom) && (showElapsedTime == true) )
                std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() <<std::endl;
            
            texture.loadFromImage(image);
            sprite.setTexture(texture);
            stateChanged = false;

        }
        window.draw(sprite);

        if (parameters.isCuteModeEnabled) {
            CuteMode::ShowCuteness(offsetX, offsetY, zoom, &window, parameters);
        }

        window.display();
    }
}
