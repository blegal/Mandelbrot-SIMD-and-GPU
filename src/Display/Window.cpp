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
#include "Window.hpp"

#if defined(__APPLE__)
    #define GL_SILENCE_DEPRECATION
#endif
#if defined(_WIN32)
    #include <windows.h>
#endif
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#if defined(__APPLE__)
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

#include <cstdio>
#include <cstdlib>

namespace gfx {


static void glfw_error(int code, const char* description)
{
    fprintf(stderr, "(EE) GLFW error %d : %s\n", code, description);
}


Window::Window(const uint32_t width, const uint32_t height, const std::string title)
{
    _width     = width;
    _height    = height;
    tex_width  = 0;
    tex_height = 0;
    texture    = 0;
    opened     = false;

    glfwSetErrorCallback( glfw_error );
    if( !glfwInit() )
    {
        fprintf(stderr, "(EE) Unable to initialize GLFW (no display available ?)\n");
        exit( EXIT_FAILURE );
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if( window == nullptr )
    {
        fprintf(stderr, "(EE) Unable to create the GLFW window\n");
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    opened = true;

    glfwMakeContextCurrent  ( window );
    glfwSwapInterval        ( 1 );
    glfwSetWindowUserPointer( window, this );

    glfwSetKeyCallback        ( window, Window::onKey    );
    glfwSetCharCallback       ( window, Window::onChar   );
    glfwSetScrollCallback     ( window, Window::onScroll );
    glfwSetMouseButtonCallback( window, Window::onButton );
    glfwSetWindowCloseCallback( window, Window::onClose  );

    glGenTextures  (1, &texture);
    glBindTexture  (GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP  );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP  );
    glPixelStorei  (GL_UNPACK_ALIGNMENT, 1);
}


Window::~Window()
{
    if( texture != 0 )
        glDeleteTextures(1, &texture);
    glfwDestroyWindow( window );
    glfwTerminate();
}


bool Window::isOpen() const
{
    return opened;
}


void Window::close()
{
    opened = false;
}


void Window::setTitle(const std::string title)
{
    glfwSetWindowTitle(window, title.c_str());
}


void Window::waitEvents(const double timeout)
{
    if( events.empty() )
        glfwWaitEventsTimeout( timeout );
    else
        glfwPollEvents();
}


bool Window::pollEvent(Event& event)
{
    if( events.empty() )
        return false;
    event = events.front();
    events.pop_front();
    return true;
}


void Window::mousePosition(double& x, double& y)
{
    int ww, wh;
    glfwGetCursorPos (window, &x,  &y );
    glfwGetWindowSize(window, &ww, &wh);
    x = x * _width  / (double)ww; // coordonnees ecran => pixels de l'image
    y = y * _height / (double)wh;
}


void Window::draw(const Image& image)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    if( (tex_width != image.width()) || (tex_height != image.height()) )
    {
        tex_width  = image.width();
        tex_height = image.height();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    }
    else
    {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tex_width, tex_height, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    }
}


void Window::display()
{
    int fw, fh;
    glfwGetFramebufferSize(window, &fw, &fh); // != taille de la fenetre sur ecran Retina / HiDPI
    glViewport(0, 0, fw, fh);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);  glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
        glTexCoord2f(0.f, 1.f); glVertex2f(-1.f, -1.f);
        glTexCoord2f(1.f, 1.f); glVertex2f( 1.f, -1.f);
        glTexCoord2f(1.f, 0.f); glVertex2f( 1.f,  1.f);
        glTexCoord2f(0.f, 0.f); glVertex2f(-1.f,  1.f);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glfwSwapBuffers( window );
}


//
// Callbacks GLFW => file d'evenements
//

void Window::onKey(GLFWwindow* w, int key, int scancode, int action, int mods)
{
    if( action == GLFW_RELEASE )
        return;
    Window* win = (Window*)glfwGetWindowUserPointer( w );
    Event e = { Event::KeyPressed, Key::Unknown, MouseButton::Left, 0.0, 0.0, 0.0 };
    switch( key )
    {
        case GLFW_KEY_ESCAPE : e.key = Key::Escape; break;
        case GLFW_KEY_UP     : e.key = Key::Up;     break;
        case GLFW_KEY_DOWN   : e.key = Key::Down;   break;
        case GLFW_KEY_LEFT   : e.key = Key::Left;   break;
        case GLFW_KEY_RIGHT  : e.key = Key::Right;  break;
        default              : return; // les lettres arrivent par onChar()
    }
    win->events.push_back( e );
}


void Window::onChar(GLFWwindow* w, unsigned int codepoint)
{
    Window* win = (Window*)glfwGetWindowUserPointer( w );
    Event e = { Event::KeyPressed, Key::Unknown, MouseButton::Left, 0.0, 0.0, 0.0 };
    switch( (codepoint >= 'a' && codepoint <= 'z') ? (codepoint - 'a' + 'A') : codepoint )
    {
        case 'A' : e.key = Key::A; break;
        case 'B' : e.key = Key::B; break;
        case 'C' : e.key = Key::C; break;
        case 'D' : e.key = Key::D; break;
        case 'H' : e.key = Key::H; break;
        case 'I' : e.key = Key::I; break;
        case 'O' : e.key = Key::O; break;
        case 'P' : e.key = Key::P; break;
        case 'R' : e.key = Key::R; break;
        case 'S' : e.key = Key::S; break;
        case 'T' : e.key = Key::T; break;
        case 'Z' : e.key = Key::Z; break;
        default  : return;
    }
    win->events.push_back( e );
}


void Window::onScroll(GLFWwindow* w, double dx, double dy)
{
    Window* win = (Window*)glfwGetWindowUserPointer( w );
    Event e = { Event::MouseWheelScrolled, Key::Unknown, MouseButton::Left, dy, 0.0, 0.0 };
    win->mousePosition(e.x, e.y);
    win->events.push_back( e );
}


void Window::onButton(GLFWwindow* w, int button, int action, int mods)
{
    if( action != GLFW_PRESS )
        return;
    Window* win = (Window*)glfwGetWindowUserPointer( w );
    Event e = { Event::MouseButtonPressed, Key::Unknown, MouseButton::Left, 0.0, 0.0, 0.0 };
    if     ( button == GLFW_MOUSE_BUTTON_RIGHT  ) e.button = MouseButton::Right;
    else if( button == GLFW_MOUSE_BUTTON_MIDDLE ) e.button = MouseButton::Middle;
    win->mousePosition(e.x, e.y);
    win->events.push_back( e );
}


void Window::onClose(GLFWwindow* w)
{
    Window* win = (Window*)glfwGetWindowUserPointer( w );
    Event e = { Event::Closed, Key::Unknown, MouseButton::Left, 0.0, 0.0, 0.0 };
    win->events.push_back( e );
}

}
