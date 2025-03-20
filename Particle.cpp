// In Particle.cpp
#include "Particle.h"
#include <SFML/Graphics.hpp>

Particle::Particle(float x, float y, float vx, float vy)
{
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
}