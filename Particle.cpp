// In Particle.cpp
#include "Particle.h"

Particle::Particle(float x, float y, float vx, float vy, int radius, sf::Color color)
{
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
    this->radius = radius;
    this->color = color;
    this->influence = 0.0f;
}



void Particle::draw(sf::RenderTexture &window)
{
    sf::CircleShape shape(radius);
    shape.setPosition(x - radius, y - radius);
    shape.setFillColor(color);
    window.draw(shape);
}