#include "Slider.h"

Slider::Slider(float x, float y, float width, float height, sf::Color color)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->color = color;
}

void Slider::draw(sf::RenderWindow &window)
{
    sf::RectangleShape rectangle(sf::Vector2f(width, height));
    sf::RectangleShape rail(sf::Vector2f(width*6, height/3));
    rectangle.setPosition(x, y);
    rail.setPosition(400, y+height/3);
    rail.setFillColor(sf::Color::White);
    rectangle.setFillColor(color);
    window.draw(rail);
    window.draw(rectangle);
}