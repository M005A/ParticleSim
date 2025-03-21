#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Particle.h"
#include "Slider.h"
#include <cmath>
using namespace std;
vector<Particle> particles;

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

double calculateDensity(const Particle &mouse)
{
    if (mouse.radius == 0)
        return 0.0;

    double density = 0.0;
    double h = mouse.radius; // Use only the mouse radius as the smoothing length
    double h2 = h * h;       // Squared for efficiency

    for (const auto &particle : particles)
    {
        float dx = particle.x - mouse.x;
        float dy = particle.y - mouse.y;
        float distanceSquared = dx * dx + dy * dy;

        // Only count particles strictly within the mouse radius
        if (distanceSquared < h2)
        {
            double weight = pow(1 - (distanceSquared / h2), 3);
            density += weight;
        }
    }

    // Normalize by the area to get density
    double area = 3.14159265358979323846 * h2;
    return density / area;
}

int main()
{
    const float GRAVITY = 9.81f * 3; // m/s²
    float sliderPos = 400.0f;

    Particle followMouse(400, 300, 0, 0, 50, sf::Color(255, 0, 0, 50));

    Slider timeSlider(sliderPos, 50, 50, 25, sf::Color::Green);
    float timeValue = sliderPos; // Initialize to match the initial slider position

    Slider sizeSlider(sliderPos, 75, 50, 25, sf::Color::Red);
    float sizeValue = sliderPos; // Initialize to match the initial slider position

    Slider numSlider(sliderPos, 100, 50, 25, sf::Color::Blue);
    float numValue = sliderPos; // Initialize to match the initial slider position

    Slider smoothingSlider(sliderPos, 125, 50, 25, sf::Color::Yellow);
    float smoothingValue = sliderPos; // Initialize to match the initial slider position

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    sf::Clock clock;
    float lastTime = 0;

    // Create render texture for blur effect
    sf::RenderTexture renderTexture;
    if (!renderTexture.create(800, 600))
    {
        std::cerr << "Failed to create render texture!" << std::endl;
        return EXIT_FAILURE;
    }

    // Load blur shader
    sf::Shader blurShader;
    bool shaderLoaded = false;
    bool blurEnabled = false;
    float blurRadius = 50.0f;

    if (sf::Shader::isAvailable())
    {
        try
        {
            if (blurShader.loadFromFile("assets/blur.frag", sf::Shader::Fragment))
            {
                shaderLoaded = true;
                blurShader.setUniform("texture", sf::Shader::CurrentTexture);
                blurShader.setUniform("blur_radius", blurRadius);
            }
            else
            {
                std::cerr << "Failed to load shader!" << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Shader exception: " << e.what() << std::endl;
        }
    }
    else
    {
        std::cerr << "Shaders not available on this system." << std::endl;
    }

    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            timeValue = timeSlider.getSliderValue(event, window);
            sizeValue = sizeSlider.getSliderValue(event, window);
            numValue = numSlider.getSliderValue(event, window);
            smoothingValue = smoothingSlider.getSliderValue(event, window);

            // Handle blur toggle on B key press
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B)
            {
                blurEnabled = !blurEnabled;
                std::cout << "Blur " << (blurEnabled ? "enabled" : "disabled") << std::endl;
            }

            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = calculateSimulationTime(clock, lastTime, timeValue);

        // Update physics
        updateParticlePhysics(particles, deltaTime, GRAVITY);

        // Clear the render texture
        renderTexture.clear();

        int targetParticleCount = static_cast<int>(1.0f + (numValue - 400) / 250.0f * 624.0f);
        if (particles.size() < targetParticleCount)
        {
            // Add new particles
            for (int i = 0; i < targetParticleCount - particles.size(); i++)
            {
                Particle particle(rand() % 800, rand() % 600, 0, 0, 5.0f, sf::Color::White);
                particles.push_back(particle);
            }
        }

        while (particles.size() > targetParticleCount)
        {
            particles.pop_back();
        }

        for (auto &particle : particles)
        {
            particle.radius = 5.0f + (sizeValue - 400) / 250.0f * 100.0f;
            particle.color = sf::Color(0, 128, 255, 255.0f - (sizeValue - 400) / 250.0f * 200.0f);
            particle.draw(renderTexture);
        }

        // Draw sliders to render texture
        timeSlider.draw(renderTexture);
        sizeSlider.draw(renderTexture);
        numSlider.draw(renderTexture);
        smoothingSlider.draw(renderTexture);

        followMouse.x = sf::Mouse::getPosition(window).x;
        followMouse.y = sf::Mouse::getPosition(window).y;
        followMouse.radius = 50.0f + (smoothingValue - 400) / 250.0f * 100.0f;
        followMouse.draw(renderTexture);
        std::cout << calculateDensity(followMouse) << endl;

        // Display the render texture
        renderTexture.display();

        // Create sprite from render texture
        sf::Sprite screenSprite(renderTexture.getTexture());

        // Clear the main window
        window.clear();

        // Apply blur if enabled and shader is available
        if (blurEnabled && shaderLoaded)
        {
            window.draw(screenSprite, &blurShader);
        }
        else
        {
            window.draw(screenSprite);
        }

        window.display();
    }

    return EXIT_SUCCESS;
}