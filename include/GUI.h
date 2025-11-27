#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "imgui.h"
#include "imgui-SFML.h"

class Body;
class Simulator;

class GUI {
public:
    // Saved selection structure for creating custom brushes
    struct SavedSelection {
        std::string name;
        std::vector<Body> bodyTemplates;
        sf::Vector2<double> centerOfMass;
    };

private:
    bool render_tree;
    bool use_colors;
    bool simple_render;
    bool render_arrows;
    int scale;
    float brightness;
    float tree_brightness;
    std::string selected_brush;
    bool selecting;
    bool triggerSelect;
    sf::Vector2f selectionBegin;
    sf::Vector2f selectionEnd;
    std::vector<Body*> selectedBodies;
    static char selectionSaveName[50];
    bool togglecontext;
    sf::Vector2f contextpos;
    Body* contextbody;
    bool focus;
    double window_width;
    double window_height;
    float timescale;
    std::vector<SavedSelection> savedSelections;

public:
    GUI();
    
    void init(sf::RenderWindow& window);
    void handleEvent(sf::Event& event, sf::RenderWindow& window);
    void update(sf::RenderWindow& window, sf::Time dt);
    void render(sf::RenderWindow& window, Simulator& space, sf::RectangleShape& selectionBox, int iterations, sf::Time dt, sf::View& view);
    void shutdown();
    
    bool getRenderTree() const { return render_tree; }
    bool getUseColors() const { return use_colors; }
    bool getSimpleRender() const { return simple_render; }
    bool getRenderArrows() const { return render_arrows; }
    int getScale() const { return scale; }
    float getBrightness() const { return brightness; }
    float getTreeBrightness() const { return tree_brightness; }
    std::string getSelectedBrush() const { return selected_brush; }
    bool isSelecting() const { return selecting; }
    bool shouldTriggerSelect() const { return triggerSelect; }
    sf::Vector2f getSelectionBegin() const { return selectionBegin; }
    sf::Vector2f getSelectionEnd() const { return selectionEnd; }
    std::vector<Body*>& getSelectedBodies() { return selectedBodies; }
    bool getFocus() const { return focus; }
    float getTimescale() const { return timescale; }
    
    void setSelecting(bool val) { selecting = val; }
    void setTriggerSelect(bool val) { triggerSelect = val; }
    void setSelectionBegin(sf::Vector2f pos) { selectionBegin = pos; }
    void setSelectionEnd(sf::Vector2f pos) { selectionEnd = pos; }
    void setFocus(bool val) { focus = val; }
    void setWindowSize(double width, double height) { window_width = width; window_height = height; }
    
    // Saved selection methods
    void saveSelection(const std::string& name, const std::vector<Body*>& bodies);
    bool isSavedSelection(const std::string& brushName) const;
    const std::vector<SavedSelection>& getSavedSelections() const { return savedSelections; }
    const SavedSelection* getSavedSelection(const std::string& name) const;
};
