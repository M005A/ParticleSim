#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Particle.h"
#include "Slider.h"

using namespace std;

float calculateSimulationTime(sf::Clock &clock, float &lastTime, float sliderPos)
{
    // Calculate time between frames
    float currentTime = clock.getElapsedTime().asSeconds();
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    // Calculate time scaling factor from slider position (400-650 → 0.2x-10.0x)
    float timeScale = 0.0f + (sliderPos - 400) / 250.0f * 10.0f;

    // Apply time scaling to delta
    deltaTime *= timeScale;

    // Cap maximum delta to avoid physics instability
    if (deltaTime > 0.1f)
        deltaTime = 0.1f;

    return deltaTime;
}

void updateParticlePhysics(std::vector<Particle> &particles, float deltaTime, float gravity)
{
    // Update physics for each particle
    for (auto &particle : particles)
    {
        // Update velocity due to gravity
        particle.vy += gravity * deltaTime;

        // Update position based on velocity
        particle.x += particle.vx * deltaTime;
        particle.y += particle.vy * deltaTime;

        // Simple floor collision
        if (particle.y > 550)
        {
            particle.y = 550;
            particle.vy *= -0.8f; // Bounce with energy loss
        }
    }
}

void drawGrid(vector<sf::RectangleShape> &rectangles, int col, int row)
{
    // Draw grid
    for (int i = 0; i < col; i++)
    {
        for (int j = 0; j < row; j++)
        {
            sf::RectangleShape rectangle(sf::Vector2f(50, 50));
            rectangle.setPosition(800 / col * i, 600 / row * j);
            rectangle.setFillColor(sf::Color::White);
            rectangle.setOutlineColor(sf::Color::Black);
            rectangle.setOutlineThickness(2);
            rectangles.push_back(rectangle);
        }
    }
}

int main()
{
    const float GRAVITY = 9.81f * 3; // m/s²
    float sliderPos = 400.0f;

    vector<Particle> particles;
    
    Slider timeSlider(sliderPos, 50, 50, 25, sf::Color::Green);
    float timeValue = sliderPos;  // Initialize to match the initial slider position

    Slider sizeSlider(sliderPos, 75, 50, 25, sf::Color::Red);
    float sizeValue = sliderPos;  // Initialize to match the initial slider position

    Slider numSlider(sliderPos, 100, 50, 25, sf::Color::Blue);
    float numValue = sliderPos;  // Initialize to match the initial slider position


    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    sf::Clock clock;
    float lastTime = 0;


    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            timeValue = timeSlider.getSliderValue(event, window);
            sizeValue = sizeSlider.getSliderValue(event, window);
            numValue = numSlider.getSliderValue(event, window);
            if (event.type == sf::Event::Closed)
                window.close();



        }

        // Calculate simulation time step
        float deltaTime = calculateSimulationTime(clock, lastTime, timeValue);

        // Update physics
        updateParticlePhysics(particles, deltaTime, GRAVITY);


        // Render
        window.clear();

        int targetParticleCount = static_cast<int>(1.0f + (numValue - 400) / 250.0f * 99.0f);
        while (particles.size() < targetParticleCount)
        {
            particles.push_back(Particle(rand() % 800, rand() % 600, 0, 0));
        }
        while (particles.size() > targetParticleCount)
        {
            particles.pop_back();
        }

        for (auto &particle : particles)
        {
           sf::CircleShape shape(2.0f + (sizeValue - 400) / 250.0f * 18.0f);
           shape.setPosition(particle.x, particle.y);
           shape.setFillColor(sf::Color::Red);
           window.draw(shape);
        }

        // Draw slider 
        timeSlider.draw(window);
        sizeSlider.draw(window);
        numSlider.draw(window);
        window.display();
    }

    return EXIT_SUCCESS;
}