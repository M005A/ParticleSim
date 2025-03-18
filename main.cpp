#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Particle.h"
#include "Circle.h"
#include "Slider.h"

using namespace std;

float calculateSimulationTime(sf::Clock &clock, float &lastTime, float sliderPos)
{
    // Calculate time between frames
    float currentTime = clock.getElapsedTime().asSeconds();
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    // Calculate time scaling factor from slider position (400-650 → 0.2x-3.0x)
    float timeScale = 0.2f + (sliderPos - 400) / 250.0f * 9.8f;

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

    vector<Particle> particles;
    vector<sf::RectangleShape> rectangles;

    drawGrid(rectangles, 25, 25);

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    sf::Clock clock;
    float lastTime = 0;
    bool dragging = false;
    float sliderPos = 400.0f;

    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle mouse events
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (event.mouseButton.x > sliderPos - 50 &&
                        event.mouseButton.x < sliderPos + 50 &&
                        event.mouseButton.y > 50 &&
                        event.mouseButton.y < 75)
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
        }

        // Calculate simulation time step
        float deltaTime = calculateSimulationTime(clock, lastTime, sliderPos);

        // Update physics
        updateParticlePhysics(particles, deltaTime, GRAVITY);

        // Update slider position if dragging
        if (dragging)
        {
            sliderPos = sf::Mouse::getPosition(window).x;
            if (sliderPos < 400)
                sliderPos = 400;
            if (sliderPos > 650)
                sliderPos = 650;
        }

        // Render
        window.clear();

        // Draw particles
        for (const auto &particle : particles)
        {
            Circle circle(particle.x, particle.y, 10, sf::Color::Red);
            circle.draw(window);
        }
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        Circle followCircle(static_cast<float>(mousePos.x - 20), static_cast<float>(mousePos.y - 20), 20, sf::Color::Red);

        // Draw rectangles (very inefficient, should be optimized)  
        for (sf::RectangleShape &rectangle : rectangles)
        {
            if (followCircle.x + 40 > rectangle.getPosition().x &&
                followCircle.x + 40 < rectangle.getPosition().x + 50 &&
                followCircle.y + 40 > rectangle.getPosition().y &&
                followCircle.y + 40 < rectangle.getPosition().y + 50)
            {
                rectangle.setFillColor(sf::Color::Green);
            }
            else
            {
                rectangle.setFillColor(sf::Color::White);
            }

            window.draw(rectangle);
        }
        followCircle.draw(window);
        // Draw slider (commented out until Slider class is properly implemented)
        // Slider slider(sliderPos, 50, 50, 25, sf::Color::Green);
        // slider.draw(window);

        window.display();
    }

    return EXIT_SUCCESS;
}