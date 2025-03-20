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

    // Create render texture for blur effect
    sf::RenderTexture renderTexture;
    if (!renderTexture.create(800, 600)) {
        std::cerr << "Failed to create render texture!" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Load blur shader
    sf::Shader blurShader;
    bool shaderLoaded = false;
    bool blurEnabled = false;
    float blurRadius = 50.0f;
    
    if (sf::Shader::isAvailable()) {
        try {
            if (blurShader.loadFromFile("assets/blur.frag", sf::Shader::Fragment)) {
                shaderLoaded = true;
                blurShader.setUniform("texture", sf::Shader::CurrentTexture);
                blurShader.setUniform("blur_radius", blurRadius);
            } else {
                std::cerr << "Failed to load shader!" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Shader exception: " << e.what() << std::endl;
        }
    } else {
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
            
            // Handle blur toggle on B key press
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B) {
                blurEnabled = !blurEnabled;
                std::cout << "Blur " << (blurEnabled ? "enabled" : "disabled") << std::endl;
            }
            
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Calculate simulation time step
        float deltaTime = calculateSimulationTime(clock, lastTime, timeValue);

        // Update physics
        updateParticlePhysics(particles, deltaTime, GRAVITY);

        // Clear the render texture
        renderTexture.clear();

        // Draw particles to render texture
        int targetParticleCount = static_cast<int>(1.0f + (numValue - 400) / 250.0f * 99.0f);
        while (particles.size() < targetParticleCount)
        {
            int randX = rand() % 800;
            int randY = rand() % 600;
            for (auto &particle : particles)
            {
                if (particle.x == randX && particle.y == randY)
                {
                    randX = rand() % 800;
                    randY = rand() % 600;
                }
            }
            particles.push_back(Particle(randX, randY, 0, 0));
        }
        while (particles.size() > targetParticleCount)
        {
            particles.pop_back();
        }

        for (auto &particle : particles)
        {
            float radius = 2.0f + (sizeValue - 400) / 250.0f * 100.0f;
            sf::CircleShape shape(radius);
            shape.setPosition(particle.x - radius, particle.y - radius);
            shape.setFillColor(sf::Color(0, 128, 255, 255.0f - (sizeValue - 400)/250.0f * 200.0f));
            renderTexture.draw(shape);
        }
        
        // Draw sliders to render texture
        timeSlider.draw(renderTexture);
        sizeSlider.draw(renderTexture);
        numSlider.draw(renderTexture);
        
        // Display the render texture
        renderTexture.display();
        
        // Create sprite from render texture
        sf::Sprite screenSprite(renderTexture.getTexture());
        
        // Clear the main window
        window.clear();
        
        // Apply blur if enabled and shader is available
        if (blurEnabled && shaderLoaded) {
            window.draw(screenSprite, &blurShader);
        } else {
            window.draw(screenSprite);
        }
        
        window.display();
    }

    return EXIT_SUCCESS;
}