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
#ifndef _GFX_WINDOW_
#define _GFX_WINDOW_

#include <deque>
#include <string>

#include "Utils/Image.hpp"

//
// Fenetre d'affichage basee sur GLFW + OpenGL (1.1, profil de compatibilite).
// Disponible sous Linux, macOS et Windows. L'API s'inspire de sf::RenderWindow
// afin de conserver la structure de la boucle d'evenements de main.cpp.
//

struct GLFWwindow;

namespace gfx {

//
// Les touches "lettres" sont reportees selon la disposition du clavier
// (AZERTY, QWERTY, ...) : on recoit le caractere tape et non la touche physique.
//
enum class Key {
    Unknown,
    A, B, C, D, H, I, O, P, R, S, T, Z,
    Escape, Up, Down, Left, Right
};

enum class MouseButton { Left, Right, Middle };

struct Event {
    enum Type { Closed, KeyPressed, MouseWheelScrolled, MouseButtonPressed };

    Type        type;
    Key         key;
    MouseButton button;
    double      delta; // deplacement de la molette (peut etre fractionnaire sur un trackpad)
    double      x;     // position de la souris, en pixels de l'image
    double      y;
};

class Window {
public:
    Window(const uint32_t width, const uint32_t height, const std::string title);

    ~Window();

    bool isOpen() const;

    void close();

    void setTitle(const std::string title);

    //
    // Attend un evenement (au plus timeout secondes) puis remplit la file
    //
    void waitEvents(const double timeout);

    bool pollEvent(Event& event);

    void draw(const Image& image);

    void display();

    static void onKey   (GLFWwindow* w, int key, int scancode, int action, int mods);
    static void onChar  (GLFWwindow* w, unsigned int codepoint);
    static void onScroll(GLFWwindow* w, double dx, double dy);
    static void onButton(GLFWwindow* w, int button, int action, int mods);
    static void onClose (GLFWwindow* w);

private:
    void mousePosition(double& x, double& y);

    GLFWwindow*       window;
    unsigned int      texture;
    uint32_t          tex_width;
    uint32_t          tex_height;
    uint32_t          _width;
    uint32_t          _height;
    bool              opened;
    std::deque<Event> events;
};

}

#endif
