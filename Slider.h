#ifndef SLIDER_H
#define SLIDER_H

#include <SFML/Graphics.hpp>

class Slider
{
public:
    Slider(float x, float y, float width, float height, sf::Color color);
    float getSliderValue(sf::Event event, sf::RenderWindow &window);
    void draw(sf::RenderWindow &window);

private:
    float x;
    float y;
    float width;
    float height;
    bool dragging;
    sf::Color color;
};

#endif // SLIDER_H