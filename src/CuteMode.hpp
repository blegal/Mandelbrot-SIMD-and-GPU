/********************
      Cute Mode
********************/

#ifndef CUTEMODE_HPP_
#define CUTEMODE_HPP_

#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <math.h>

#include "Utils/Settings.hpp"
#include "Utils/StringUtils.hpp"

#define CAT_COUNT 8

using namespace std;

class CuteMode {
private:

public:
	static std::vector<sf::Texture> cat;

	static void LoadCuteness();
	static void DrawCat(int catnb, double x, double y, double size, float rotation, double offsetX, double offsetY, double zoom, sf::RenderWindow* window,  Settings params);
	static void ShowCuteness(double offsetX, double offsetY, double zoom, sf::RenderWindow* window, Settings params);
};

#endif /*CUTEMODE_HPP_*/
