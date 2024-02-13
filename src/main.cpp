#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include "Simulator.h"
#include <thread>
#include <chrono>
#include "imgui-SFML.h"
#include "imgui.h"

// a function to create a custom galaxy or cluster shape based on certain arguments
// sim: the simulator object in which to instantiate the bodies
// starnum: the total number of stars to create
// starmass: the mass of each star on average
// c_mass: the mass of a central star, e.g. the sun or a black hole. If set to 0, this object wont be created
// rad: the radius of the galaxy or cluster
// rotspeed: the tangential velocity of each planet
// cx: the x position of the center of the cluster
// cy: the y position of the center of the cluster
// c_velx: the velocity of the cluster in the x direction relative to other galaxies
// c_vely: the velocity of the cluster in the y direction relative to other galaxies
// radial: the outward radial velocity of the stars
// massvar: the range of values that the starmass could take.
void addGalaxy(Simulator& sim, double starnum, double starmass, double c_mass, double rad, double rotspeed, double cx, double cy, double c_velx, double c_vely, double radial=0, double massvar = 10.0){
    int num_threads = 16;
    std::vector<std::thread> threads(num_threads);
    std::vector<Body*> ptrarr((int)starnum,nullptr);

    // 16 threads are created to iterate through 1/16th of the starnum
    for(int i = 0; i < num_threads; i++){
        threads[i] = std::thread([&](int i, int n){
            for(int j = i*n; j < ((i+1)*n); j++) {
                if(j >= starnum) break;

                // the following maths uses polar coordinates to position each of the stars randomly in space
                double radius = rad;
                double rnd = rand();
                double r = radius * sqrt( pow((double)rand()/RAND_MAX, 2));
                double theta = ((double)rnd/RAND_MAX) * 2 * M_PI;
                double x = r * cos(theta);
                double y = r * sin(theta);

                // the following maths calculates the velocity of each star based on its angle to the center
                double rnd2 = (((double)rand()/ RAND_MAX)*massvar);
                double velx = -cos(M_PI*0.5 - ( ((double)rnd/ RAND_MAX) * 2*M_PI )) *  200 * (2- r/radius) * rotspeed;
                double vely =  sin(M_PI*0.5 - ( ((double)rnd/ RAND_MAX) * 2*M_PI )) *  200 * (2- r/radius) * rotspeed;

                // the following applies the radial velocity to each star
                double velradx = radial*x;
                double velrady = radial*y;
                ptrarr[j] = new Body(starmass * rnd2 * 10,20 * rnd2,sf::Vector2<double>(x+cx, y+cy), sf::Vector2<double>(velx + c_velx + velradx,vely + c_vely + velrady));
            }

        },i,std::ceil((double)starnum/num_threads));
    }

    // the 16 threads are before exiting scope to prevent memory issues
    for(int i = 0; i < num_threads; i++) {
        threads[i].join();
    }

    // nullcount is recorded as a precautionary measure of how many threads failed to execute
    int nullcount = 0;
    for(int i = 0; i < starnum; i++){
        if(ptrarr[i] == nullptr){
            nullcount++;
        }
        sim.addBody(*ptrarr[i]);
    }
    std::cout << nullcount << std::endl;

    // since all bodies are now added to the simulation, the original pointers can be deleted to avoid memory leaks
    for(Body* b : ptrarr)
        delete b;
    ptrarr.clear();

    if(c_mass) sim.addBody(Body(c_mass,10,sf::Vector2<double>(cx, cy), sf::Vector2<double>(c_velx, c_vely)));
}


int main()
{
    double speed = 0.5;    // the speed multiplier of moving around the simulation
    double window_width  = 1920;
    double window_height = 1080;

    // the render window is instantiated
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "OrbitSim");
    window.setPosition(sf::Vector2i(0,0));
    ImGui::SFML::Init(window);
    window.setFramerateLimit(60);
    int sizex = 1920*1;
    int sizey = 1080*1;

    // a view is instantiated to navigate the simulation space
    sf::View view(sf::FloatRect(-sizex/2.0f, -sizey/2.0f, sizex, sizey));
    window.setView(view);


    // the simulator is instantiated with a large bounding box, but not too large
    // to avoid creating a recursion depth error
    Simulator space(10000000);
    float timescale  = 1; // a multiplier to the deltatime to alter the simulation speed
    bool paused = false; // to pause or unpause the simulation
    bool render_tree = true; // to activate the rendering of the quad tree
    bool use_colors = true; // whether to use color highlighting or black and white
    bool simple_render = false; // whether to render with pixels or polygons
    bool render_arrows = false; // whether to render arrows for velocity and acceleration on each object
    int scale = 4; // the scale of pixels in simple rendering mode
    float brightness = 1; // the brightness of the simulation colours (includes blending)
    float tree_brightness = 0.04; // the brightness of the quadtree (includes blending)
    std::string selected_brush = ""; // the currently selected brush, set to "" for no brush selected
    sf::RectangleShape selectionBox; // the selection box object for drag selecting multiple bodies
    bool selecting = false; // to check whether objects are being actively selected or not
    bool triggerSelect = false; // to notify when a change has been made in the selection
    sf::Vector2f selectionBegin; // the top left coordinate of the selection box
    sf::Vector2f selectionEnd; // the bottom right coordinate of the selection box
    std::vector<Body*> selectedBodies; // a list of pointers to all bodies that have been selected
    static char selectionSaveName[50] = ""; // the name to save a selection as in storage

    bool togglecontext = false; // to check whether a single selection popup is open
    sf::Vector2f contextpos; // the position of the topmost context popup
    Body* contextbody = nullptr; // the selected single body


    // a preliminary check to see whether any bodies are directly coinciding with each other. 
    // if this is the case we notify the user, since the simulation cannot proceed with direct collision
    for(int i = 0; i < space.bodies.size(); i++)
    {
        for(int j = i+1; j < space.bodies.size(); j++)  {
            if(space.bodies[i].position.x == space.bodies[j].position.x and space.bodies[i].position.y == space.bodies[j].position.y)
                throw std::domain_error("two bodies cannot have the same coordinate!");
        }

    }
    std::cout << "OBJECTS LOADED" << std::endl;

    // to check whether the whole program is being focused or not. to be used later when registering key presses
    bool focus = true;

    sf::Clock deltaClock; // a timer to measure the length of each physics frame
    sf::Time dt; // the variable to store each frame's measured time in 
    sf::Uint8* pix; // a vector of pixels to render during simple mode
    int iterations = 0; // a count of how many iterations the simulation has gone through

    // begin the main game loop
    while (window.isOpen())
    {

        sf::Event event; // an sfml event to detect user input
        while (window.pollEvent(event)) // while the event queue is non empty each frame
        {
            ImGui::SFML::ProcessEvent(window, event);
            // if the X key on the window is pressed we close the window
            if (event.type == sf::Event::Closed)                       window.close(); 
            // if the space key is pressed we toggle the pause state of the simulation
            if(event.type == sf::Event::KeyReleased and event.key.code == sf::Keyboard::Space) {
                paused = !paused;
            }

            // handling left mouse button presses
            if(event.type == sf::Event::MouseButtonReleased and event.key.code == sf::Mouse::Left){
                // this gets the position of the mouse IN SIMULATION COORDINATES instead of window coordinates
                sf::Vector2<double> pos = (sf::Vector2<double>)window.mapPixelToCoords(sf::Mouse::getPosition());

                // if the mouse is not interacting with any ImGui objects we can proceed to simulation interaction
                if(!ImGui::IsAnyItemActive() and !ImGui::IsAnyItemFocused() and !ImGui::IsAnyItemHovered()) {
                    // checks each possible brush type and creates the associated cluster
                    if (selected_brush == "single")
                        addGalaxy(space, 0, 0, 1000, 1000, 0, pos.x, pos.y, -0, 000, .0, 1.0);
                    if (selected_brush == "black_hole")
                        addGalaxy(space, 0, 10000, 100000000000, 1000, 0, pos.x, pos.y, -0, 000, .0, 1.0);
                    if (selected_brush == "cluster100")
                        addGalaxy(space, 100, 1000, 1000, 100, 1, pos.x, pos.y, 0, 000, 0.0, 1.0);
                    if (selected_brush == "cluster1k")
                        addGalaxy(space, 1000, 1000, 1000, 1000, 1, pos.x, pos.y, 0, 000, 0.0, 1.0);
                    if (selected_brush == "cluster10k")
                        addGalaxy(space, 10000, 1000, 1000, 10000, 1, pos.x, pos.y, 0, 000, 0.0, 1.0);
                }
            }

            // handling right mouse button presses and assumes multibody selection
            if(event.type == sf::Event::MouseButtonPressed and event.key.code == sf::Mouse::Right){
                selecting = true;
                selectionBegin = window.mapPixelToCoords(sf::Mouse::getPosition());
            }

            // handling right mouse button releases to check whether selection is multi or single
            if(event.type == sf::Event::MouseButtonReleased and event.key.code == sf::Mouse::Right) {
                selecting = false;

                // checks whether the mouse has dragged and therefore multiselect
                if(ImGui::IsMouseDragging(ImGuiMouseButton_Right)){
                    std::cout << "opened popup" << std::endl;
                    sf::Vector2f TL(std::min(selectionBegin.x, selectionEnd.x), std::min(selectionBegin.y, selectionEnd.y));
                    sf::Vector2f BR(std::max(selectionBegin.x, selectionEnd.x), std::max(selectionBegin.y, selectionEnd.y));

                    // loops through each body and checks whether its position is within the bounding box of the selection
                    for(auto& body : space.bodies){
                        body.selected = false;
                        if(body.position.x < BR.x and body.position.x > TL.x and body.position.y < BR.y and body.position.y > TL.y){
                            selectedBodies.push_back(&body);
                            body.selected = true;
                        }
                    }

                    // checks whether the multiselect made any successful selections at all
                    if(!selectedBodies.empty())
                        triggerSelect = true;

                    if(paused) paused = false;
                }

            }



            // handling window resizing by changing the simulation view width to the new size
            if(event.type == sf::Event::Resized){
                std::cout << event.size.width << " " << event.size.height << std::endl;
                view.setSize(view.getSize().x * (event.size.width/window_width) , view.getSize().y * (event.size.height/window_height));
                window_width = event.size.width;
                window_height = event.size.height;
            }

        }


        // handling keyboard input for simulation space navigation ONLY if the window is focused
        if(focus) {
            // Q to zoom in
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                view.setSize(view.getSize().x * (1 - 1 * dt.asSeconds()), view.getSize().y * (1 - 1 * dt.asSeconds()));
            // E to zoom out
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                view.setSize(view.getSize().x * (1 + 1 * dt.asSeconds()), view.getSize().y * (1 + 1 * dt.asSeconds()));
            // W to move up
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                view.setCenter(view.getCenter().x, view.getCenter().y - speed * view.getSize().y * dt.asSeconds());
            // A to move left
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                view.setCenter(view.getCenter().x - speed * view.getSize().x * dt.asSeconds(), view.getCenter().y);
            // S to move down
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                view.setCenter(view.getCenter().x, view.getCenter().y + speed * view.getSize().y * dt.asSeconds());
            // D to move right
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                view.setCenter(view.getCenter().x + speed * view.getSize().x * dt.asSeconds(), view.getCenter().y);
        }


        // refreshes the view each frame as it might have changed due to simulation navigation
        window.setView(view);







        ImGui::SFML::Update(window, dt);

        // creates the main menu bar in the top of the screen
        if(ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if(ImGui::MenuItem("Quit"))
                {
                    window.close();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }


        // handles the context menu for the single body selection
        // the single body popup only opens if the selectedBodies list is empty and therefore
        // indicates that there was no mouse dragging involved
        if(selectedBodies.empty()) {

            // creates the imgui popup
            if (ImGui::BeginPopupContextVoid("itemcheck", ImGuiPopupFlags_MouseButtonRight)) {

                // the following code locates the body to select based on the mouse position
                sf::Vector2f pos;
                if (!togglecontext) {
                    // the mouse position
                    pos = window.mapPixelToCoords(ImGui::GetWindowPos());

                    // window attributes
                    sf::Vector2f origin = sf::Vector2f(view.getCenter().x - view.getSize().x / 2,
                                                       view.getCenter().y - view.getSize().y / 2);
                    sf::Vector2f size = sf::Vector2f(view.getSize().x, view.getSize().y);
                    std::cout << pos.x << " " << pos.y << std::endl;

                    // simple render mode has a different selection method than the polygon mode
                    if (simple_render) {

                        // get the pixel in the simple render grid where the mouse is pointing
                        int gridx = (pos.x - origin.x) / (size.x / 1920.0);
                        gridx -= gridx % scale;
                        int gridy = (pos.y - origin.y) / (size.y / 1080.0);
                        gridy -= gridy % scale;
                        sf::Vector2i gridpos = {gridx, gridy};

                        // record any bodies that are within 10 pixels of the mouse coordinate
                        std::vector<Body *> results;
                        for (auto &body: space.bodies) {

                            sf::Vector2f bpos = (sf::Vector2f) body.position;
                            int bodyx = (bpos.x - origin.x) / (size.x / 1920.0);
                            bodyx -= bodyx % scale;
                            int bodyy = (bpos.y - origin.y) / (size.y / 1080.0);
                            bodyy -= bodyy % scale;
                            sf::Vector2i bodypos = {bodyx, bodyy};
                            if (std::abs(bodypos.x - gridpos.x) <= 100 and std::abs(bodypos.y - gridpos.y) <= 100) {
                                results.push_back(&body);
                            }
                        }

                        std::cout << results.size() << std::endl;

                        // find the closest body of these recorded selections within 10 pixels
                        float mindist = FLT_MAX;
                        if (!results.empty()) {
                            for (int i = 0; i < (int) results.size(); i++) {
                                float dist = std::sqrt(std::pow(results[i]->position.x - pos.x, 2) +
                                                       std::pow(results[i]->position.y - pos.y, 2));
                                if (dist < mindist) {
                                    contextbody = results[i];
                                    mindist = dist;
                                }
                            }
                        } else {
                            contextbody = nullptr;
                        }
                    } else {

                        // records any bodies which are close enough to the mouse position to be within 
                        // that body's polygon radius
                        std::vector<Body *> results;
                        for (auto &body: space.bodies) {
                            if (std::pow(body.position.x - pos.x, 2) + std::pow(body.position.y - pos.y, 2) <=
                                std::pow(body.radius, 2)) {
                                results.push_back(&body);
                            }
                        }

                        std::cout << results.size() << std::endl;

                        // calculates the closest body of these recorded bodies
                        float mindist = FLT_MAX;
                        if (!results.empty()) {
                            for (int i = 0; i < (int) results.size(); i++) {
                                float dist = std::sqrt(std::pow(results[i]->position.x - pos.x, 2) +
                                                       std::pow(results[i]->position.y - pos.y, 2));
                                if (dist < mindist) {
                                    contextbody = results[i];
                                    mindist = dist;
                                }
                            }
                        } else {
                            contextbody = nullptr;
                        }
                    }
                }

                // initialises the properties of the context menu 
                // if a body was not found during the selection process, we skip this step
                if (contextbody == nullptr) {
                    ImGui::CloseCurrentPopup();
                } else {
                    // set up the table to fit the space of the context width
                    ImGui::BeginTable("tmp", 2, 0);
                    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
                    
                    // begin the first row of the table as a title header and a close button
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("Inspect Entity");
                    ImGui::TableNextColumn();
                    if (ImGui::Button("close")){
                        ImGui::CloseCurrentPopup();
                    }
                    // include the escape key as an option to exit the popup
                    if(ImGui::IsKeyReleased(ImGuiKey_Escape)){
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndTable();
                    
                    // set up a table to edit the main properties of the body
                    ImGui::BeginChild("entityicon", ImVec2(100, 100), true);
                    // a status icon to see the highlight color of the current body
                    ImColor contextcolor = IM_COL32(contextbody->shape.getFillColor().r,
                                                    contextbody->shape.getFillColor().g,
                                                    contextbody->shape.getFillColor().b, 255);
                    ImGui::GetWindowDrawList()->AddCircleFilled(
                            ImVec2(ImGui::GetItemRectMin().x + 50, ImGui::GetItemRectMin().y + 50), 45, contextcolor);
                    ImGui::EndChild();
                    ImGui::SameLine();

                    // a table to modify the body attributes
                    ImGui::BeginTable("entityops", 2, 0);
                    ImGui::TableSetupColumn("hi1");
                    ImGui::TableSetupColumn("hi2");
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("mass"); // modify the mass attribute
                    ImGui::TableNextColumn();
                    ImGui::SetNextItemWidth(100);
                    ImGui::InputDouble("##mass", &contextbody->mass);
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("radius"); // modify the body radius
                    ImGui::TableNextColumn();
                    ImGui::SetNextItemWidth(100);
                    ImGui::InputDouble("##radius", &contextbody->radius);
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();

                    // modifies the position of the body
                    ImGui::Text("position"); 
                    ImGui::TableNextColumn();
                    float posinp[2] = {(float) contextbody->position.x, (float) contextbody->position.y};
                    ImGui::SetNextItemWidth(100);
                    ImGui::InputFloat2("##position", posinp);
                    contextbody->position = {posinp[0], posinp[1]}; // this reduces from double to float <:(

                    // modifies the velocity of the body
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("velocity");
                    ImGui::TableNextColumn();
                    float velinp[2] = {(float) contextbody->velocity.x, (float) contextbody->velocity.y};
                    ImGui::SetNextItemWidth(100);
                    ImGui::InputFloat2("##velocity", velinp);
                    contextbody->velocity = {velinp[0], velinp[1]}; // this reduces from double to float <:(

                    // modifies the acceleration of the body
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("acceleration");
                    ImGui::TableNextColumn();
                    float accinp[2] = {(float) contextbody->acceleration.x, (float) contextbody->acceleration.y};
                    ImGui::SetNextItemWidth(100);
                    ImGui::InputFloat2("##acceleration", accinp);
                    contextbody->acceleration = {accinp[0], accinp[1]}; // this reduces from double to float <:(


                    ImGui::EndTable();

                }


                togglecontext = true;
                ImGui::EndPopup();
            } else {
                togglecontext = false;
                contextbody = nullptr;
                //std::cout << "hi" << std::endl;
            }
        }

        // if a drag selection has been completed, we open the multiselect context popup
        if(triggerSelect){
            ImGui::OpenPopup("selectionpopup");
            triggerSelect = false;
        }

        // defines the multiselect context popup
        if (ImGui::BeginPopup("selectionpopup")) {

            // the popup can either be closed manually or with the escape key
            if(ImGui::Button("close")){
                ImGui::CloseCurrentPopup();
            }
            if(ImGui::IsKeyReleased(ImGuiKey_Escape)){
                ImGui::CloseCurrentPopup();
            }

            // an option to save the selection
            // opens a modal to save the body with a name
            if (ImGui::Button("save as...##saveselection")) {
                ImGui::OpenPopup("saveselectionmodal");
                focus = false;
            }
            if (ImGui::BeginPopupModal("saveselectionmodal", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::SetWindowPos(ImVec2(window_width / 2 - ImGui::GetWindowSize().x / 2,
                                           window_height / 2 - ImGui::GetWindowSize().y / 2));
                if (ImGui::Button("save selection")) {
                    ImGui::CloseCurrentPopup();
                    focus = true;
                }
                ImGui::InputTextWithHint("##selectionsavename", "enter object name...", selectionSaveName,
                                         IM_ARRAYSIZE(selectionSaveName));
                ImGui::EndPopup();
            }

            // an option to delete the selected bodies
            if (ImGui::Button("delete##deleteselection")) {
                std::vector<Body> newBodies;
                for (auto &body: space.bodies) {
                    if (!body.selected) newBodies.push_back(body);
                }
                space.bodies = newBodies;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    else{
        if(!selectedBodies.empty()){
            for(auto& body : space.bodies) body.selected = false;
            selectedBodies.clear();
        }
    }

        // initialising the bottom bar of the simulation
        ImGui::SetNextWindowPos(ImVec2(0.0,window.getSize().y-window_height/10.0));
        ImGui::SetNextWindowSize(ImVec2(window.getSize().x,window_height/10.0));

        // the following flags indicate that the bottom bar should have no titlebar
        // not be resizable or moveable, and not have a scrollbar
        ImGuiWindowFlags flags=0;
        flags |= ImGuiWindowFlags_NoTitleBar;
        flags |= ImGuiWindowFlags_NoResize;
        flags |= ImGuiWindowFlags_NoMove;
        flags |= ImGuiWindowFlags_NoScrollbar;


        // begin declaring the bottom bar
        ImGui::Begin("Bottom Bar", NULL, flags);

        // the settings button opens the popup modal for settings
        if(ImGui::Button("Settings", ImVec2(window_height/10.0 - 15.0, window_height/10.0 - 15.0))){
            ImGui::OpenPopup("settings");
        }

        // declare the settings modal
        if(ImGui::BeginPopupModal("settings", NULL, ImGuiWindowFlags_Tooltip)){
            if(ImGui::Button("close")){
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        ImGui::SameLine();

        ImGui::BeginGroup();

        // begin declaring each of the button options
        // each slider option has its respective tooltip embedded describing what the button does
        ImGui::SameLine();
        ImGui::BeginGroup();
            ImGui::Checkbox("render Quad Tree?", &render_tree);
            if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
                ImGui::BeginTooltip();
                ImGui::Text("enable to render the debug quad-tree responsible for the Barnes-Hut algorithm");
                ImGui::EndTooltip();
            }
            ImGui::Checkbox("Use Colors?", &use_colors);
            if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
                ImGui::BeginTooltip();
                ImGui::Text("enable to render the stars on a 'heatmap-like' colour scale");
                ImGui::EndTooltip();
            }
            ImGui::Checkbox("Simple rendering?", &simple_render);
            if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
                ImGui::BeginTooltip();
                ImGui::Text("enable to render using pixels instead of polygons (can break on some systems)");
                ImGui::EndTooltip();
            }
            ImGui::BeginDisabled(simple_render);
            ImGui::Checkbox("Render arrows?", &render_arrows);
            ImGui::EndDisabled();
            if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
                ImGui::BeginTooltip();
                ImGui::Text("enable to render arrows for acceleration and velocity on shapes");
                ImGui::EndTooltip();
            }
        ImGui::EndGroup();
        ImGui::SameLine();

        // begin declaring each of the slider options
        // each slider option has its respective tooltip embedded describing what the slider does
        ImGui::BeginChild("sliders", ImVec2(window_width/4.0, window_height/10.0), false);
            ImGui::SliderInt("resolution", &scale,1,40);
            if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
                ImGui::BeginTooltip();
                ImGui::Text("sets the resolution of simple rendering, higher -> lower resolution");
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,200,2002,255)); // changes the color of the text
                ImGui::Text("It is recommended that you use common factors of 1920x1080, such as:");
                ImGui::Text("(1,2,3,4,5,6,8,10,12,15,20,24,30,40)");
                ImGui::PopStyleColor();
                ImGui::EndTooltip();
            }
            ImGui::SliderFloat("star brightness", &brightness, 0.1, 1);
            if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
                ImGui::BeginTooltip();
                ImGui::Text("sets the brightness of simple rendering stars by modifying its alpha value");
                ImGui::EndTooltip();
            }
            ImGui::BeginDisabled(!render_tree);
            ImGui::SliderFloat("tree brightness",&tree_brightness, 0.01, 1.0);
            ImGui::EndDisabled();
            if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
                ImGui::BeginTooltip();
                ImGui::Text("Sets the brightness of each new QuadTree square drawn by modifying its alpha value");
                ImGui::EndTooltip();
            }
            ImGui::SliderFloat("timescale",&timescale, 0.001, 2.0);
            if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
                ImGui::BeginTooltip();
                ImGui::Text("Changes the speed of the simulation. Higher->faster");
                ImGui::EndTooltip();
            }
        ImGui::EndChild();
        ImGui::EndGroup();
        ImGui::SameLine();

        // begins the brushes menu
        // each brush button changes the 'selected_brush' variable
        // the menu may overflow, but the scrollbars have still been disabled for aesthetic
        ImGui::BeginChild("brushes", ImVec2(window_width/4.0, window_height/10.0 -15.0), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::BeginDisabled(!selected_brush.empty() and selected_brush != "single");
        if(ImGui::Button("single", ImVec2(window_height/10.0 - 30.0, window_height/10.0 - 30.0)))
            if(selected_brush=="single")
                selected_brush="";
            else
                selected_brush = "single";
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::BeginDisabled(!selected_brush.empty() and selected_brush != "black_hole");
        if(ImGui::Button("black_hole", ImVec2(window_height/10.0 - 30.0, window_height/10.0 - 30.0)))
            if(selected_brush=="black_hole")
                selected_brush="";
            else
                selected_brush = "black_hole";
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::BeginDisabled(!selected_brush.empty() and selected_brush != "cluster100");
        if(ImGui::Button("cluster100", ImVec2(window_height/10.0 - 30.0, window_height/10.0 - 30.0)))
            if(selected_brush=="cluster100")
                selected_brush="";
            else
                selected_brush = "cluster100";
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::BeginDisabled(!selected_brush.empty() and selected_brush != "cluster1k");
        if(ImGui::Button("cluster1k", ImVec2(window_height/10.0 - 30.0, window_height/10.0 - 30.0)))
            if(selected_brush=="cluster1k")
                selected_brush="";
            else
                selected_brush = "cluster1k";
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::BeginDisabled(!selected_brush.empty() and selected_brush != "cluster10k");
        if(ImGui::Button("cluster10k", ImVec2(window_height/10.0 - 30.0, window_height/10.0 - 30.0)))
            if(selected_brush=="cluster10k")
                selected_brush="";
            else
                selected_brush = "cluster10k";
        ImGui::EndDisabled();
        ImGui::EndChild();

        ImGui::End();

        // begin declaring the HUD menu for settings
        // this window has the additional flags of being transparent and autosizing to its contents
        // the window will contain the fps count, number of entities, iterations, the mouse position 
        // and the current brush
        ImGuiWindowFlags flags2 = flags;
        flags2 |= ImGuiWindowFlags_NoBackground;
        flags2 |= ImGuiWindowFlags_AlwaysAutoResize;
        ImGui::Begin("debug corner", NULL, flags2);
        ImGui::SetWindowPos(ImVec2(0.0,15.0));
        ImGui::Text("FPS %.2f", 1/dt.asSeconds() );
        ImGui::Text("Entity Count %i", (int)space.bodies.size());
        ImGui::Text("Iterations: %i", iterations);
        ImGui::Text("Mouse pos: (%i, %i)", (int)window.mapPixelToCoords(sf::Mouse::getPosition()).x, (int)window.mapPixelToCoords(sf::Mouse::getPosition()).y);
        ImGui::Text("Current brush: (%s)", selected_brush.c_str());
        ImGui::End();

        
        window.clear();


        // creates the selection box if the 'selecting' variable is active
        // the selectionbox is a white rectangular box with zero fill
        if(selecting){
            sf::Vector2f curmouse = window.mapPixelToCoords(sf::Mouse::getPosition());
            selectionBox.setFillColor(sf::Color::Transparent);
            selectionBox.setOutlineThickness(view.getSize().x/window_width);
            selectionBox.setOutlineColor(sf::Color::White);
            selectionBox.setSize(sf::Vector2f(curmouse.x-selectionBegin.x, curmouse.y-selectionBegin.y));
            selectionBox.setPosition(selectionBegin);
            selectionEnd = curmouse;
        }
        else if(selectedBodies.empty()){
            selectionBox = sf::RectangleShape();
        }

        // SIMPLE RENDERING CODE
        // since arrays are static, we need to create a separate array
        // for each resolution
        sf::Texture tex1920; tex1920.create(1920,1080);
        sf::Texture tex0960; tex0960.create(1920/2,1080/2);
        sf::Texture tex0640; tex0640.create(1920/3,1080/3);
        sf::Texture tex0480; tex0480.create(1920/4,1080/4);
        sf::Texture tex0384; tex0384.create(1920/5,1080/5);
        sf::Texture tex0320; tex0320.create(1920/6,1080/6);
        sf::Texture tex0240; tex0240.create(1920/8,1080/8);
        sf::Texture tex0192; tex0192.create(1920/10,1080/10);
        sf::Texture tex0160; tex0160.create(1920/12,1080/12);
        sf::Texture tex0128; tex0128.create(1920/15,1080/15);


        // creates the main texture
        sf::Texture tex;
        tex.create(1920,1080);
        sf::Sprite sprite;
        sprite.setTexture(tex);

        // the simple rendering algorithm iterates through each planet, and converts its
        // coordinates to a location on a pixel grid by floor division of the coordinates
        // any planets with positions outside the grid are ignored
        if(simple_render) {
            sf::Vector2f origin = sf::Vector2f(view.getCenter().x - view.getSize().x / 2,
                                               view.getCenter().y - view.getSize().y / 2);
            sf::Vector2f size = sf::Vector2f(view.getSize().x, view.getSize().y);
            pix = new sf::Uint8[1920 * 1080 * 4];
            memset(pix, 0, 1920 * 1080 * 4 * sizeof(sf::Uint8));

            double maxvel=-1;
            for(auto &body : space.bodies){
                sf::Vector2<double> pos = body.position;
                if (pos.x < origin.x or pos.x > origin.x + size.x) continue;
                if (pos.y < origin.y or pos.y > origin.y + size.y) continue;
                double vel = std::sqrt(std::pow(body.velocity.x,2) + std::pow(body.velocity.y,2));
                maxvel=std::max(vel,maxvel);
            }
            for (auto &body: space.bodies) {
                sf::Vector2<double> pos = body.position;

                if (pos.x < origin.x or pos.x > origin.x + size.x) continue;
                if (pos.y < origin.y or pos.y > origin.y + size.y) continue;

                double vel = std::sqrt(std::pow(body.velocity.x,2) + std::pow(body.velocity.y,2));
                body.shape.setFillColor(Body::convert_to_rgb(0,maxvel,vel));
                if (!use_colors) body.shape.setFillColor(sf::Color::White);


                int gridx = (pos.x - origin.x) / (size.x / 1920.0);
                gridx -= gridx % scale;
                int gridy = (pos.y - origin.y) / (size.y / 1080.0);
                gridy -= gridy % scale;

                if (&body == contextbody) {
                    body.shape.setFillColor(sf::Color::White);
                }
                if(body.selected){
                    body.shape.setFillColor(sf::Color::White);
                }

                for (int i = 0; i < scale; i++) {
                    for (int j = 0; j < scale; j++) {
                        pix[4 * (((gridy + i) * 1920) + gridx + j) + 0] = body.shape.getFillColor().r;
                        pix[4 * (((gridy + i) * 1920) + gridx + j) + 1] = body.shape.getFillColor().g;
                        pix[4 * (((gridy + i) * 1920) + gridx + j) + 2] = body.shape.getFillColor().b;
                        pix[4 * (((gridy + i) * 1920) + gridx + j) + 3] = std::min(255,(int)pix[4 * (((gridy + i) * 1920) + gridx + j) + 3]+ (int)(255 * brightness));
                    }
                }
            }

            tex.update(pix);

            sprite.setOrigin(960, 540);
            sprite.setPosition(view.getCenter().x, view.getCenter().y);
            sprite.setScale((float) view.getSize().x / 1920.0f, (float) view.getSize().y / 1080.0f);

            window.draw(sprite);
        }
        else {
            // regular rendering mode simply loops through each object and 
            // renders its respective circleobject
            for(auto& body : space.bodies) {
                if(body.selected)
                    body.shape.setFillColor(sf::Color::White);
                if (&body == contextbody) {
                    body.shape.setOutlineColor(sf::Color::White);
                    body.shape.setOutlineThickness(body.shape.getRadius() / 10.0);
                } else {
                    body.shape.setOutlineThickness(0.0);
                }
            }
            space.draw(window);
        }


        // update the quadTree, increment every force
        // and increment every position/velocity of the bodies
        space.updateTree(tree_brightness);
        space.updateForces(false);
        space.updateBodies(dt.asMicroseconds() * timescale * paused);

        // optionally also render the quadTree
        if(render_tree) space.drawTree(window);

        // render the selectionbox 
        window.draw(selectionBox);
        ImGui::SFML::Render(window);
        window.display();

        // since the array is static and may contain pointers and therefore need to be 
        // freed from memory each frame of the program 
        if(simple_render) delete[] pix;

        // restart the deltaclock
        dt = deltaClock.restart();
        if(paused) iterations++;
    }

    ImGui::SFML::Shutdown();
    return 0;
}
