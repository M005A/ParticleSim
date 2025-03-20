// Include guards
#ifndef PARTICLE_H
#define PARTICLE_H
#include <SFML/Graphics.hpp>

class Particle
{
    public:

        Particle(float x, float y, float vx, float vy, int radius, sf::Color color);
        void draw(sf::RenderTexture &window);
        float smoothingFunction(float radius, float dst);
        
        
        float radius;
        sf::Color color;
        float x;
        float y;
        float vx;
        float vy;
        float influence;
};

#endif // PARTICLE_H