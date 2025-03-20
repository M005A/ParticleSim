cmake_minimum_required(VERSION 3.10)
project(ParticleSim)

set(CMAKE_CXX_STANDARD 17)

# Find SFML 2.5.1
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(ParticleSim main.cpp Slider.cpp Slider.h Particle.cpp Particle.h) 

# Link SFML libraries
target_link_libraries(ParticleSim sfml-graphics sfml-window sfml-system)
