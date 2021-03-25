#include "ColorXaoS.hpp"


ColorXaoS::ColorXaoS(int max_iters) : ColorMap("SMOOTH")
{
    setIters(max_iters);
}


void ColorXaoS::setIters(int max_iters)
{
    MAX = max_iters; // memorisation du nombre d'iterations max.
/*
    var MAXENTRIES = 65536;
    var segmentsize = 8;
    var setsegments = Math.floor((MAXENTRIES + 3) / segmentsize);
    var nsegments = Math.floor(255 / segmentsize);

    uint32_t segments[30][3] = {;
    {0, 0, 0},
    {120, 119, 238},
    {24, 7, 25},
    {197, 66, 28},
    {29, 18, 11},
    {135, 46, 71},
    {24, 27, 13},
    {241, 230, 128},
    {17, 31, 24},
    {240, 162, 139},
    {11, 4, 30},
    {106, 87, 189},
    {29, 21, 14},
    {12, 140, 118},
    {10, 6, 29},
    {50, 144, 77},
    {22, 0, 24},
    {148, 188, 243},
    {4, 32, 7},
    {231, 146, 14},
    {10, 13, 20},
    {184, 147, 68},
    {13, 28, 3},
    {169, 248, 152},
    {4, 0, 34},
    {62, 83, 48},
    {7, 21, 22},
    {152, 97, 184},
    {8, 3, 12},
    {247, 92, 235},
    {31, 32, 16}
    };
    var i, y;
    var r, g, b;
    var rs, gs, bs;
    var palette = [];

    for (i = 0; i < setsegments; i++) {
        const uint8_t r  = segments[i % nsegments][0];
        const uint8_t g  = segments[i % nsegments][1];
        const uint8_t b  = segments[i % nsegments][2];

        const uint8_t rs = (segments[(i + 1) % setsegments % nsegments][0] - r) / segmentsize;
        const uint8_t gs = (segments[(i + 1) % setsegments % nsegments][1] - g) / segmentsize;
        const uint8_t bs = (segments[(i + 1) % setsegments % nsegments][2] - b) / segmentsize;

        for (y = 0; y < segmentsize; y++) {
            colors[i] = sf::Color(r, g, b);
            r += rs;
            g += gs;
            b += bs;
        }
    }
    return new Uint32Array(palette);

    //////////////

    MAX = max_iters; // memorisation du nombre d'iterations max.

    for (int i=0; i <= MAX; ++i) {
        const float t = (float)i/(float)max_iters;
        const int r   = (int)std::round( 9.f * (1.0f-t) * t * t * t               * 255.0f);
        const int g   = (int)std::round(15.f * (1.0f-t) * (1.0f-t) * t * t        * 255.0f);
        const int b   = (int)std::round(8.5f * (1.0f-t) * (1.0f-t) * (1.0f-t) * t * 255.0f);
        colors[i] = sf::Color(r, g, b);
    }
    */
}


ColorXaoS::~ColorXaoS()
{

}
