#pragma once

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

class Simulator;

class GUI {
private:
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
    // Simulation parameters
    bool reset_simulation = false;
    int particle_count = 10000;
    float time_step = 0.001f;
    bool pause_simulation = false;
    bool show_tree = false;
    float zoom_level = 1.0f;
    sf::Vector2f camera_position = {0.0f, 0.0f};
    
    // Galaxy parameters
    double galaxy_star_num = 10000;
    double galaxy_star_mass = 1.0;
    double galaxy_c_mass = 10000000;
    double galaxy_rad = 500;
    double galaxy_rot_speed = 0.001;
    double galaxy_cx = 0;
    double galaxy_cy = 0;
    double galaxy_c_velx = 0;
    double galaxy_c_vely = 0;
    double galaxy_radial = 0;
    double galaxy_mass_var = 10.0;
    double galaxy_gap = 0.0;

public:
    void init(sf::RenderWindow& window);
    void handleEvent(sf::Event& event, sf::RenderWindow& window);
    void update(sf::RenderWindow& window, sf::Time deltaTime);
    void render();
    void shutdown();
    
    // Getters for main.cpp to read simulation parameters
    bool shouldResetSimulation();
    int getParticleCount() const { return particle_count; }
    float getTimeStep() const { return time_step; }
    bool isPaused() const { return pause_simulation; }
    bool shouldShowTree() const { return show_tree; }
    float getZoomLevel() const { return zoom_level; }
    sf::Vector2f getCameraPosition() const { return camera_position; }
    
    // Galaxy parameters getters
    double getGalaxyStarNum() const { return galaxy_star_num; }
    double getGalaxyStarMass() const { return galaxy_star_mass; }
    double getGalaxyCMass() const { return galaxy_c_mass; }
    double getGalaxyRad() const { return galaxy_rad; }
    double getGalaxyRotSpeed() const { return galaxy_rot_speed; }
    double getGalaxyCx() const { return galaxy_cx; }
    double getGalaxyCy() const { return galaxy_cy; }
    double getGalaxyCVelx() const { return galaxy_c_velx; }
    double getGalaxyCVely() const { return galaxy_c_vely; }
    double getGalaxyRadial() const { return galaxy_radial; }
    double getGalaxyMassVar() const { return galaxy_mass_var; }
    double getGalaxyGap() const { return galaxy_gap; }
    
    // Setters for camera control
    void setCameraPosition(sf::Vector2f pos) { camera_position = pos; }
    void setZoomLevel(float zoom) { zoom_level = zoom; }
};