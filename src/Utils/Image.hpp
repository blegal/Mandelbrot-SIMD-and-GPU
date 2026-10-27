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
#ifndef _GFX_IMAGE_
#define _GFX_IMAGE_

#include <cstdint>
#include <string>
#include <vector>

//
// Petite couche graphique independante de toute bibliotheque de fenetrage.
// Elle reprend volontairement l'API de sf::Color / sf::Image (SFML) afin que
// les moteurs de calcul et les palettes restent portables (CPU, CUDA, Metal).
//

namespace gfx {

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color() : r(0), g(0), b(0), a(255) { }
    Color(const uint8_t _r, const uint8_t _g, const uint8_t _b, const uint8_t _a = 255) : r(_r), g(_g), b(_b), a(_a) { }
};

class Image {
public:
    Image();

    void create(const uint32_t width, const uint32_t height, const Color& color = Color());

    inline void setPixel(const uint32_t x, const uint32_t y, const Color& color)
    {
        pixels[y * _width + x] = color;
    }

    inline Color getPixel(const uint32_t x, const uint32_t y) const
    {
        return pixels[y * _width + x];
    }

    uint32_t width () const { return _width;  }
    uint32_t height() const { return _height; }

    //
    // Pointeur sur les pixels au format RGBA8 (utilisable par glTexImage2D)
    //
    const uint8_t* getPixelsPtr() const;

    //
    // Sauvegarde au format PNG, PPM ou BMP (deduit de l'extension du fichier)
    //
    bool saveToFile(const std::string& filename) const;

private:
    bool savePNG(const std::string& filename) const;
    bool savePPM(const std::string& filename) const;
    bool saveBMP(const std::string& filename) const;

    uint32_t _width;
    uint32_t _height;
    std::vector<Color> pixels;
};

}

#endif
