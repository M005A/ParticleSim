#ifndef CIRCLE_H
#define CIRCLE_H

#include <SFML/Graphics.hpp>

class Circle
{
public:
    float x;
    float y;
    float radius;
    sf::Color color;
    Circle(float x, float y, float radius, sf::Color color);
    void draw(sf::RenderWindow &window);
};

#endif // CIRCLE_H