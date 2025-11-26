#include "GUI.h"
#include "Simulator.h"
#include <iostream>
#include <climits>

void GUI::init(sf::RenderWindow& window) {
    ImGui::SFML::Init(window);
}

bool GUI::shouldResetSimulation() {
    if (reset_simulation) {
        reset_simulation = false;
        return true;
    }
    return false;
}

void GUI::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    ImGui::SFML::ProcessEvent(window, event);
}

void GUI::update(sf::RenderWindow& window, sf::Time deltaTime) {
    ImGui::SFML::Update(window, deltaTime);
}

void GUI::render() {
    // Main menu bar
    if(ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("Quit")) {
                // This will be handled by main.cpp
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Bottom bar with controls
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoScrollbar;
    
    ImGui::Begin("Bottom Bar", NULL, flags);
    
    // Settings button
    if(ImGui::Button("Settings", ImVec2(108 - 15.0, 108 - 15.0))) {
        ImGui::OpenPopup("settings");
    }
    
    // Settings modal
    if(ImGui::BeginPopupModal("settings", NULL, ImGuiWindowFlags_Tooltip)) {
        if(ImGui::Button("close")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    
    ImGui::SameLine();
    ImGui::BeginGroup();
    
    // Checkboxes
    ImGui::SameLine();
    ImGui::BeginGroup();
    
    bool render_tree_temp = show_tree;
    ImGui::Checkbox("render Quad Tree?", &render_tree_temp);
    show_tree = render_tree_temp;
    if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
        ImGui::BeginTooltip();
        ImGui::Text("enable to render the debug quad-tree responsible for the Barnes-Hut algorithm");
        ImGui::EndTooltip();
    }
    
    static bool use_colors = true;
    ImGui::Checkbox("Use Colors?", &use_colors);
    if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
        ImGui::BeginTooltip();
        ImGui::Text("enable to render the stars on a 'heatmap-like' colour scale");
        ImGui::EndTooltip();
    }
    
    static bool simple_render = false;
    ImGui::Checkbox("Simple rendering?", &simple_render);
    if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
        ImGui::BeginTooltip();
        ImGui::Text("enable to render using pixels instead of polygons (can break on some systems)");
        ImGui::EndTooltip();
    }
    
    ImGui::BeginDisabled(simple_render);
    static bool render_arrows = false;
    ImGui::Checkbox("Render arrows?", &render_arrows);
    ImGui::EndDisabled();
    if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
        ImGui::BeginTooltip();
        ImGui::Text("enable to render arrows for acceleration and velocity on shapes");
        ImGui::EndTooltip();
    }
    
    ImGui::EndGroup();
    ImGui::SameLine();
    
    // Sliders
    ImGui::BeginChild("sliders", ImVec2(1920/4.0, 1080/10.0), false);
    
    static int scale = 4;
    ImGui::SliderInt("resolution", &scale, 1, 40);
    if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
        ImGui::BeginTooltip();
        ImGui::Text("sets the resolution of simple rendering, higher -> lower resolution");
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,200,2002,255));
        ImGui::Text("It is recommended that you use common factors of 1920x1080, such as:");
        ImGui::Text("(1,2,3,4,5,6,8,10,12,15,20,24,30,40)");
        ImGui::PopStyleColor();
        ImGui::EndTooltip();
    }
    
    static float brightness = 1.0f;
    ImGui::SliderFloat("star brightness", &brightness, 0.1, 1);
    if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
        ImGui::BeginTooltip();
        ImGui::Text("sets the brightness of simple rendering stars by modifying its alpha value");
        ImGui::EndTooltip();
    }
    
    ImGui::BeginDisabled(!show_tree);
    static float tree_brightness = 0.04f;
    ImGui::SliderFloat("tree brightness", &tree_brightness, 0.01, 1.0);
    ImGui::EndDisabled();
    if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
        ImGui::BeginTooltip();
        ImGui::Text("Sets the brightness of each new QuadTree square drawn by modifying its alpha value");
        ImGui::EndTooltip();
    }
    
    ImGui::SliderFloat("timescale", &time_step, 0.001, 2.0);
    if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
        ImGui::BeginTooltip();
        ImGui::Text("Changes the speed of the simulation. Higher->faster");
        ImGui::EndTooltip();
    }
    
    ImGui::EndChild();
    ImGui::EndGroup();
    ImGui::SameLine();
    
    // Brushes menu
    ImGui::BeginChild("brushes", ImVec2(1920/4.0, 1080/10.0 - 15.0), true, ImGuiWindowFlags_NoScrollbar);
    
    static std::string selected_brush = "";
    ImGui::BeginDisabled(!selected_brush.empty() && selected_brush != "single");
    if(ImGui::Button("single", ImVec2(1080/10.0 - 30.0, 1080/10.0 - 30.0))) {
        if(selected_brush == "single") {
            selected_brush = "";
        } else {
            selected_brush = "single";
        }
    }
    ImGui::EndDisabled();
    
    ImGui::SameLine();
    ImGui::BeginDisabled(!selected_brush.empty() && selected_brush != "galaxy");
    if(ImGui::Button("galaxy", ImVec2(1080/10.0 - 30.0, 1080/10.0 - 30.0))) {
        if(selected_brush == "galaxy") {
            selected_brush = "";
        } else {
            selected_brush = "galaxy";
        }
    }
    ImGui::EndDisabled();
    
    ImGui::EndChild();
    ImGui::SameLine();
    
    // Pause/Play controls
    ImGui::BeginGroup();
    if(ImGui::Button(pause_simulation ? "Play" : "Pause", ImVec2(100, 50))) {
        pause_simulation = !pause_simulation;
    }
    
    if(ImGui::Button("Reset", ImVec2(100, 50))) {
        reset_simulation = true;
    }
    ImGui::EndGroup();
    
    ImGui::End();

    // Galaxy creation window
    if(selected_brush == "galaxy") {
        ImGui::Begin("Galaxy Parameters");
        
        ImGui::InputDouble("Star Count", &galaxy_star_num);
        ImGui::InputDouble("Star Mass", &galaxy_star_mass);
        ImGui::InputDouble("Central Mass", &galaxy_c_mass);
        ImGui::InputDouble("Radius", &galaxy_rad);
        ImGui::InputDouble("Rotation Speed", &galaxy_rot_speed);
        ImGui::InputDouble("Center X", &galaxy_cx);
        ImGui::InputDouble("Center Y", &galaxy_cy);
        ImGui::InputDouble("Velocity X", &galaxy_c_velx);
        ImGui::InputDouble("Velocity Y", &galaxy_c_vely);
        ImGui::InputDouble("Radial Velocity", &galaxy_radial);
        ImGui::InputDouble("Mass Variation", &galaxy_mass_var);
        ImGui::InputDouble("Gap", &galaxy_gap);
        
        ImGui::End();
    }
}

void GUI::shutdown() {
    ImGui::SFML::Shutdown();
}