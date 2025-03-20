#include "Slider.h"

Slider::Slider(float x, float y, float width, float height, sf::Color color)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->color = color;
    dragging = false;

}

float Slider::getSliderValue(sf::Event event, sf::RenderWindow &window)
{
    // Handle mouse events
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            if (event.mouseButton.x > x - 50 &&
                event.mouseButton.x < x + 50 &&
                event.mouseButton.y > y &&
                event.mouseButton.y < y + 25)
            {
                dragging = true;
            }
        }
    }

    if (event.type == sf::Event::MouseButtonReleased)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            dragging = false;
        }
    }


    if (dragging)
    {
        x = sf::Mouse::getPosition(window).x;
        if (x < 400)
            x = 400;
        if (x > 650)
            x = 650;
    }
    return x;
}

void Slider::draw(sf::RenderTexture &renderTexture)
{
    sf::RectangleShape rectangle(sf::Vector2f(width, height));
    sf::RectangleShape rail(sf::Vector2f(width*6, height/3));
    rectangle.setPosition(x, y);
    rail.setPosition(400, y+height/3);
    rail.setFillColor(sf::Color::White);
    rectangle.setFillColor(color);
    renderTexture.draw(rail);
    renderTexture.draw(rectangle);
}