#ifndef SLIDER_H
#define SLIDER_H

#include <SFML/Graphics.hpp>

class Slider
{
public:
    Slider(float x, float y, float width, float height, sf::Color color);
    void draw(sf::RenderWindow &window);

private:
    float x;
    float y;
    float width;
    float height;
    sf::Color color;
};

#endif // SLIDER_H