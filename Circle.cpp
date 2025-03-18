#include "Circle.h"

Circle::Circle(float x, float y, float radius, sf::Color color)
{
    this->x = x;
    this->y = y;
    this->radius = radius;
    this->color = color;
}

void Circle::draw(sf::RenderWindow &window)
{
    sf::CircleShape circle(radius);
    circle.setPosition(x, y);
    //circle.setFillColor(color);
    circle.setOutlineThickness(1);
    circle.setOutlineColor(color);
    window.draw(circle);
}