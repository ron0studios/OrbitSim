#include "GUI.h"
#include "Simulator.h"
#include "Body.h"

char GUI::selectionSaveName[50] = "";

GUI::GUI() {
    render_tree = true;
    use_colors = true;
    simple_render = false;
    render_arrows = false;
    scale = 4;
    brightness = 1.0f;
    tree_brightness = 0.04f;
    selected_brush = "";
    selecting = false;
    triggerSelect = false;
    selectionBegin = sf::Vector2f(0.0f, 0.0f);
    selectionEnd = sf::Vector2f(0.0f, 0.0f);
    togglecontext = false;
    contextpos = sf::Vector2f(0.0f, 0.0f);
    contextbody = nullptr;
    focus = true;
    window_width = 1920;
    window_height = 1080;
    timescale = 1.0f;
}

void GUI::init(sf::RenderWindow& window) {
    ImGui::SFML::Init(window);
}

void GUI::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    ImGui::SFML::ProcessEvent(window, event);
}

void GUI::update(sf::RenderWindow& window, sf::Time dt) {
    ImGui::SFML::Update(window, dt);
}

void GUI::render(sf::RenderWindow& window, Simulator& space, sf::RectangleShape& /* selectionBox */, int iterations, sf::Time dt, sf::View& view) {
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

    if(selectedBodies.empty()) {

        if (ImGui::BeginPopupContextVoid("itemcheck", ImGuiPopupFlags_MouseButtonRight)) {

            sf::Vector2f pos;
            if (!togglecontext) {
                pos = window.mapPixelToCoords(ImGui::GetWindowPos());

                sf::Vector2f origin = sf::Vector2f(view.getCenter().x - view.getSize().x / 2,
                                                   view.getCenter().y - view.getSize().y / 2);
                sf::Vector2f size = sf::Vector2f(view.getSize().x, view.getSize().y);

                if (simple_render) {

                    int gridx = (pos.x - origin.x) / (size.x / 1920.0);
                    gridx -= gridx % scale;
                    int gridy = (pos.y - origin.y) / (size.y / 1080.0);
                    gridy -= gridy % scale;
                    sf::Vector2i gridpos = {gridx, gridy};

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

                    std::vector<Body *> results;
                    for (auto &body: space.bodies) {
                        if (std::pow(body.position.x - pos.x, 2) + std::pow(body.position.y - pos.y, 2) <=
                            std::pow(body.radius, 2)) {
                            results.push_back(&body);
                        }
                    }

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

            if (contextbody == nullptr) {
                ImGui::CloseCurrentPopup();
            } else {
                ImGui::BeginTable("tmp", 2, 0);
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
                
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Inspect Entity");
                ImGui::TableNextColumn();
                if (ImGui::Button("close")){
                    ImGui::CloseCurrentPopup();
                }
                if(ImGui::IsKeyReleased(ImGuiKey_Escape)){
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndTable();
                
                ImGui::BeginChild("entityicon", ImVec2(100, 100), true);
                ImColor contextcolor = IM_COL32(contextbody->shape.getFillColor().r,
                                                contextbody->shape.getFillColor().g,
                                                contextbody->shape.getFillColor().b, 255);
                ImGui::GetWindowDrawList()->AddCircleFilled(
                        ImVec2(ImGui::GetItemRectMin().x + 50, ImGui::GetItemRectMin().y + 50), 45, contextcolor);
                ImGui::EndChild();
                ImGui::SameLine();

                ImGui::BeginTable("entityops", 2, 0);
                ImGui::TableSetupColumn("hi1");
                ImGui::TableSetupColumn("hi2");
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("mass");
                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(100);
                ImGui::InputDouble("##mass", &contextbody->mass);
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("radius");
                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(100);
                ImGui::InputDouble("##radius", &contextbody->radius);
                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                ImGui::Text("position"); 
                ImGui::TableNextColumn();
                float posinp[2] = {(float) contextbody->position.x, (float) contextbody->position.y};
                ImGui::SetNextItemWidth(100);
                ImGui::InputFloat2("##position", posinp);
                contextbody->position = {posinp[0], posinp[1]};

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("velocity");
                ImGui::TableNextColumn();
                float velinp[2] = {(float) contextbody->velocity.x, (float) contextbody->velocity.y};
                ImGui::SetNextItemWidth(100);
                ImGui::InputFloat2("##velocity", velinp);
                contextbody->velocity = {velinp[0], velinp[1]};

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("acceleration");
                ImGui::TableNextColumn();
                float accinp[2] = {(float) contextbody->acceleration.x, (float) contextbody->acceleration.y};
                ImGui::SetNextItemWidth(100);
                ImGui::InputFloat2("##acceleration", accinp);
                contextbody->acceleration = {accinp[0], accinp[1]};


                ImGui::EndTable();

            }


            togglecontext = true;
            ImGui::EndPopup();
        } else {
            togglecontext = false;
            contextbody = nullptr;
        }
    }

    if(triggerSelect){
        ImGui::OpenPopup("selectionpopup");
        triggerSelect = false;
    }

    if (ImGui::BeginPopup("selectionpopup")) {

        if(ImGui::Button("close")){
            ImGui::CloseCurrentPopup();
        }
        if(ImGui::IsKeyReleased(ImGuiKey_Escape)){
            ImGui::CloseCurrentPopup();
        }

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

    ImGui::SetNextWindowPos(ImVec2(0.0,window.getSize().y-window_height/10.0));
    ImGui::SetNextWindowSize(ImVec2(window.getSize().x,window_height/10.0));

    ImGuiWindowFlags flags=0;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoResize;
    flags |= ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoScrollbar;


    ImGui::Begin("Bottom Bar", NULL, flags);

    if(ImGui::Button("Settings", ImVec2(window_height/10.0 - 15.0, window_height/10.0 - 15.0))){
        ImGui::OpenPopup("settings");
    }

    if(ImGui::BeginPopupModal("settings", NULL, ImGuiWindowFlags_Tooltip)){
        if(ImGui::Button("close")){
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::SameLine();

    ImGui::BeginGroup();

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

    ImGui::BeginChild("sliders", ImVec2(window_width/4.0, window_height/10.0), false);
        ImGui::SliderInt("resolution", &scale,1,40);
        if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
            ImGui::BeginTooltip();
            ImGui::Text("sets the resolution of simple rendering, higher -> lower resolution");
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,200,2002,255));
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
        ImGui::SliderFloat("timescale", &timescale, 0.0, 5.0);
        if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
            ImGui::BeginTooltip();
            ImGui::Text("Controls the simulation speed multiplier (0 = paused, 1 = normal speed)");
            ImGui::EndTooltip();
        }
    ImGui::EndChild();
    ImGui::EndGroup();
    ImGui::SameLine();

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
}

void GUI::shutdown() {
    ImGui::SFML::Shutdown();
}
