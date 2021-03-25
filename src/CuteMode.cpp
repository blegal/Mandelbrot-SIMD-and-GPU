/********************
      Cute Mode
********************/

#include "CuteMode.hpp"

std::vector<sf::Texture> CuteMode::cat;

void CuteMode::LoadCuteness() {
  for (int i = 0 ; i < CAT_COUNT ; i++) {
    sf::Texture texture;
    if (texture.loadFromFile("cte/cat" + ToString(i) + ".png")){
      cat.push_back(texture);
    } else {
      cout << "\033[33m" << endl; //unix only
      cerr << "warning: failed to load \"cte/cat" + ToString(i) + ".png\", file ignored";
      cout << "\033[0m"  << endl;
    }
  }
}

void CuteMode::DrawCat(
        int catnb, double x, double y, double size, float rotation, double offsetX, double offsetY,
        double zoom, sf::RenderWindow* window, Settings params
) {
  if (cat.size() > catnb) {
    float zoomCoeff = size*0.004/zoom;

    sf::Vector2<double> windowCatPosition;
    windowCatPosition.x = (x - offsetX)/zoom + (double)(params.Width ())/2.0;
    windowCatPosition.y = (y - offsetY)/zoom + (double)(params.Height())/2.0;

    sf::Sprite cat_sprite;
    cat_sprite.setTexture(CuteMode::cat[catnb]);
    cat_sprite.setPosition(windowCatPosition.x, windowCatPosition.y);
    cat_sprite.setRotation(rotation);
    cat_sprite.setScale(zoomCoeff, zoomCoeff);

    window->draw(cat_sprite);
  }
}

void CuteMode::ShowCuteness(double offsetX, double offsetY, double zoom, sf::RenderWindow* window, Settings params) {
  DrawCat(0, -1.070, -0.325, 0.05,   -1, offsetX, offsetY, zoom, window, params);
  DrawCat(1, -0.087, -0.890, 0.05,   20, offsetX, offsetY, zoom, window, params);
  DrawCat(2,  0.074, -0.678, 0.05,   20, offsetX, offsetY, zoom, window, params);
  DrawCat(3, -1.340, -0.112, 0.04,    0, offsetX, offsetY, zoom, window, params);
  DrawCat(4, -1.610, -0.078, 0.02,    0, offsetX, offsetY, zoom, window, params);
  DrawCat(5, -0.785, -0.154, 0.02,    0, offsetX, offsetY, zoom, window, params);
  DrawCat(6,  0.290, -0.009, 0.04,    6, offsetX, offsetY, zoom, window, params);
  DrawCat(7, -0.355, -0.657, 0.015, -20, offsetX, offsetY, zoom, window, params);
  DrawCat(7, -0.1781778397961463, -1.0885170933841756, 0.000007, 0, offsetX, offsetY, zoom, window, params);//autozoom
}
