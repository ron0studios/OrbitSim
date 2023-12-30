#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include "Simulator.h"
#include <thread>
#include <chrono>
#include "imgui-SFML.h"
#include "imgui.h"

void addGalaxy(Simulator& sim, double starnum, double starmass, double c_mass, double rad, double rotspeed, double cx, double cy, double c_velx, double c_vely, double radial=0, double massvar = 10.0){


    int num_threads = 16;
    std::vector<std::thread> threads(num_threads);
    std::vector<Body*> ptrarr((int)starnum,nullptr);


    /*
     * NOTE OF BUG
     * YOU CANT USE A REGULAR FOR LOOP BECAUSE THE "i" VALUE IS DELETED AT THE END OF THE LOOP DUMB
     */

    for(int i = 0; i < num_threads; i++){
        //std::cout << i << std::endl;
        threads[i] = std::thread([&](int i, int n){
            //std::cout << "\t\t" << i << " " << num_threads << std::endl;
            for(int j = i*n; j < ((i+1)*n); j++) {
                if(j >= starnum) break;

                //double dx = (((double)rand()/ RAND_MAX) * 28284) -14142;
                //double dy = (((double)rand()/ RAND_MAX) * 28284) -14142;

                double radius = rad;
                double rnd = rand();
                double r = radius * sqrt( pow((double)rand()/RAND_MAX, 2));
                double theta = ((double)rnd/RAND_MAX) * 2 * M_PI;
                double x = r * cos(theta);
                double y = r * sin(theta);

                double rnd2 = (((double)rand()/ RAND_MAX)*massvar);
                double velx = -cos(M_PI*0.5 - ( ((double)rnd/ RAND_MAX) * 2*M_PI )) *  200 * (2- r/radius) * rotspeed;
                double vely =  sin(M_PI*0.5 - ( ((double)rnd/ RAND_MAX) * 2*M_PI )) *  200 * (2- r/radius) * rotspeed;

                double velradx = radial*x;
                double velrady = radial*y;
                ptrarr[j] = new Body(starmass * rnd2 * 10,20 * rnd2,sf::Vector2<double>(x+cx, y+cy), sf::Vector2<double>(velx + c_velx + velradx,vely + c_vely + velrady));
                //sim.addBody(Body(starmass * rnd2 * 10,20 * rnd2,sf::Vector2<double>(x+cx, y+cy), sf::Vector2<double>(velx + c_velx + velradx,vely + c_vely + velrady)));
            }

            //calcForce(bodies[j]);
        },i,std::ceil((double)starnum/num_threads));
    }
    //std::cout << threads[0] << std::endl;

    for(int i = 0; i < num_threads; i++) {
        threads[i].join();
    }

    int nullcount = 0;
    for(int i = 0; i < starnum; i++){
        //std::cout << "do" << std::endl;
        if(ptrarr[i] == nullptr){
            //std::cout << "\t" << i << std::endl;
            nullcount++;
        }
        sim.addBody(*ptrarr[i]);
    }
    std::cout << nullcount << std::endl;

    for(Body* b : ptrarr)
        delete b;
    ptrarr.clear();


    /*

    for(int i = 0; i < starnum; i++) {
        //double dx = (((double)rand()/ RAND_MAX) * 28284) -14142;
        //double dy = (((double)rand()/ RAND_MAX) * 28284) -14142;

        double radius = rad;
        double rnd = rand();
        double r = radius * sqrt( pow((double)rand()/RAND_MAX, 2));
        double theta = ((double)rnd/RAND_MAX) * 2 * M_PI;
        double x = r * cos(theta);
        double y = r * sin(theta);

        double rnd2 = (((double)rand()/ RAND_MAX)*massvar);
        double velx = -cos(M_PI*0.5 - ( ((double)rnd/ RAND_MAX) * 2*M_PI )) *  200 * (2- r/radius) * rotspeed;
        double vely =  sin(M_PI*0.5 - ( ((double)rnd/ RAND_MAX) * 2*M_PI )) *  200 * (2- r/radius) * rotspeed;

        double velradx = radial*x;
        double velrady = radial*y;
        sim.addBody(Body(starmass * rnd2 * 10,20 * rnd2,sf::Vector2<double>(x+cx, y+cy), sf::Vector2<double>(velx + c_velx + velradx,vely + c_vely + velrady)));
    }
     */
    if(c_mass) sim.addBody(Body(c_mass,10,sf::Vector2<double>(cx, cy), sf::Vector2<double>(c_velx, c_vely)));
}


int main()
{
    //srand(time(0));
    double speed = 0.5;
    double window_width  = 1920;
    double window_height = 1080;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "OrbitSim");
    window.setPosition(sf::Vector2i(0,0));
    ImGui::SFML::Init(window);
    window.setFramerateLimit(60);
    int sizex = 1920*1;
    int sizey = 1080*1;
    sf::View view(sf::FloatRect(-sizex/2.0f, -sizey/2.0f, sizex, sizey));
    window.setView(view);

    Simulator space(10000000);
    float timescale  = 1;
    bool paused = false;
    bool render_tree = true;
    bool use_colors = true;
    bool simple_render = true;
    bool render_arrows = false;
    bool can_place = true;
    int scale = 4;
    float brightness = 1;
    float tree_brightness = 0.04;
    std::string selected_brush = "";

    bool togglecontext = false;
    sf::Vector2f contextpos;
    Body* contextbody = nullptr;



    //addGalaxy(space, 10000, 1000, 1000, 10000, 5, -00000, -00000, -0, 000,.4, 1.0);
    //addGalaxy(space, 10000, 1000, 1000, 10000, 5, -50000, -50000, 1000, 000,.4, 1.0);
    //addGalaxy(space, 10000, 1000, 1000, 10000, 0, -00000, -00000, -0, 000,.0, 1.0);

    //addGalaxy(space, 100, 1000, 1000, 100, 1, -00000, -00000, 0, 000,0.0, 1.0);
    //addGalaxy(space, 100, 1000, 1000, 100, 1, -1000, -1000, 1000, 000,0.0, 1.0);
    //addGalaxy(space, 100, 1000, 1000, 100, 1, 1000, 1000, 0, 000,0.0, 1.0);
    //addGalaxy(space, 100, 1000, 1000, 100, 1, 1000, -1000, 0, 000,0.0, 1.0);
    //addGalaxy(space, 100, 1000, 1000, 100, 1, -1000, 1000, 0, 000,0.0, 1.0);

    //addGalaxy(space, 100, 1000, 1000, 100, 1, 10000, -00000, 0, 000,0.0, 1.0);
    //addGalaxy(space, 100, 1000, 1000, 100, 1, 10000-1000, -1000, 1000, 000,0.0, 1.0);
    //addGalaxy(space, 100, 1000, 1000, 100, 1, 10000+1000, 1000, 0, 000,0.0, 1.0);
    //addGalaxy(space, 100, 1000, 1000, 100, 1, 10000+1000, -1000, 0, 000,0.0, 1.0);
    //addGalaxy(space, 100, 1000, 1000, 100, 1, 10000-1000, 1000, 0, 000,0.0, 1.0);



    for(int i = 0; i < space.bodies.size(); i++)
    {
        for(int j = i+1; j < space.bodies.size(); j++)  {
            if(space.bodies[i].position.x == space.bodies[j].position.x and space.bodies[i].position.y == space.bodies[j].position.y)
                throw std::domain_error("two bodies cannot have the same coordinate!");
        }

    }
    std::cout << "OBJECTS LOADED" << std::endl;

    bool focus = true;

    sf::Clock deltaClock;
    sf::Time dt;
    sf::Uint8* pix;
    int iterations = 0;


    while (window.isOpen())
    {
        //shader.setUniform("currentTexture", sf::Shader::CurrentTexture);
        //std::cout << space.bodies.size() << std::endl;
        //shader.setUniform("blur_radius", 0.00001f);


        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);
            //if(event.type == sf::Event::GainedFocus) focus = true;
            //if(event.type == sf::Event::LostFocus) focus = false;
            //if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))  window.close();
            if (event.type == sf::Event::Closed)                       window.close();
            if(event.type == sf::Event::KeyReleased and event.key.code == sf::Keyboard::Space)
                paused = !paused;



            if(event.type == sf::Event::MouseButtonReleased and event.key.code == sf::Mouse::Left){
                sf::Vector2<double> pos = (sf::Vector2<double>)window.mapPixelToCoords(sf::Mouse::getPosition());
                if((float)sf::Mouse::getPosition().y/window.getSize().y > 0.9) continue;
                //addGalaxy(space, 100, 10000, 0, 1000, 0, pos.x, pos.y, -0, 000,.0, 1.0);

                if(can_place) {
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


        }


        if(focus) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                view.setSize(view.getSize().x * (1 - 1 * dt.asSeconds()), view.getSize().y * (1 - 1 * dt.asSeconds()));
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                view.setSize(view.getSize().x * (1 + 1 * dt.asSeconds()), view.getSize().y * (1 + 1 * dt.asSeconds()));
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                view.setCenter(view.getCenter().x, view.getCenter().y - speed * view.getSize().y * dt.asSeconds());
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                view.setCenter(view.getCenter().x - speed * view.getSize().x * dt.asSeconds(), view.getCenter().y);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                view.setCenter(view.getCenter().x, view.getCenter().y + speed * view.getSize().y * dt.asSeconds());
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                view.setCenter(view.getCenter().x + speed * view.getSize().x * dt.asSeconds(), view.getCenter().y);

            //view.setCenter(view.getCenter().x + speed*view.getSize().x*dt.asSeconds(), view.getCenter().y);
        }
        window.setView(view);





        ImGui::SFML::Update(window, dt);

        /*
        if(ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if(ImGui::MenuItem("New"))
                {
                    //Do something
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
         */

        //ImGui::SetNextWindowSize(ImVec2(300,300));
        if(ImGui::BeginPopupContextVoid("itemcheck"))
        {
            can_place = false;


            sf::Vector2f pos;
            if(!togglecontext){
                pos = window.mapPixelToCoords(ImGui::GetWindowPos());
                sf::Vector2f origin = sf::Vector2f(view.getCenter().x - view.getSize().x / 2,
                                                   view.getCenter().y - view.getSize().y / 2);
                sf::Vector2f size = sf::Vector2f(view.getSize().x, view.getSize().y);
                std::cout << pos.x << " " << pos.y << std::endl;

                if(simple_render){
                    int gridx = (pos.x - origin.x) / (size.x / 1920.0);
                    gridx -= gridx % scale;
                    int gridy = (pos.y - origin.y) / (size.y / 1080.0);
                    gridy -= gridy % scale;
                    sf::Vector2i gridpos = {gridx, gridy};

                    std::vector<Body*> results;
                    for(auto &body : space.bodies){

                        sf::Vector2f bpos = (sf::Vector2f)body.position;
                        int bodyx = (bpos.x - origin.x) / (size.x / 1920.0);
                        bodyx -= bodyx % scale;
                        int bodyy = (bpos.y - origin.y) / (size.y / 1080.0);
                        bodyy -= bodyy % scale;
                        sf::Vector2i bodypos = {bodyx, bodyy};
                        if( std::abs(bodypos.x-gridpos.x) <= 100 and std::abs(bodypos.y - gridpos.y) <= 100){
                            results.push_back(&body);
                        }
                    }

                    std::cout << results.size() << std::endl;

                    float mindist = FLT_MAX;
                    if(!results.empty()){
                        for(int i =0 ; i < (int)results.size(); i++){
                            float dist = std::sqrt(std::pow(results[i]->position.x-pos.x,2)+std::pow(results[i]->position.y-pos.y, 2));
                            if(dist < mindist){
                                contextbody = results[i];
                                mindist = dist;
                            }
                        }
                    }
                    else{
                        contextbody = nullptr;
                    }
                }
                else {
                    std::vector<Body*> results;
                    for(auto &body : space.bodies) {
                               if(std::pow(body.position.x-pos.x,2) + std::pow(body.position.y-pos.y,2) <= std::pow(body.radius,2)){
                                   results.push_back(&body);
                               }
                    }

                    std::cout << results.size() << std::endl;

                    float mindist = FLT_MAX;
                    if(!results.empty()){
                        for(int i =0 ; i < (int)results.size(); i++){
                            float dist = std::sqrt(std::pow(results[i]->position.x-pos.x,2)+std::pow(results[i]->position.y-pos.y, 2));
                            if(dist < mindist){
                                contextbody = results[i];
                                mindist = dist;
                            }
                        }
                    }
                    else{
                        contextbody = nullptr;
                    }
                }
            }


            if(contextbody == nullptr){
                ImGui::CloseCurrentPopup();
            }
            else
            {
                ImGui::BeginTable("tmp",2,0);
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Inspect Entity");
                ImGui::TableNextColumn();
                if(ImGui::Button("close"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndTable();

                ImGui::BeginChild("hi", ImVec2(100,100), true);
                ImGui::EndChild();
                ImGui::SameLine();
                ImGui::BeginTable("entityops",2,0);
                ImGui::TableSetupColumn("hi" );
                ImGui::TableSetupColumn("hi");
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
                float posinp[2] = {(float)contextbody->position.x, (float)contextbody->position.y};
                ImGui::SetNextItemWidth(100);
                ImGui::InputFloat2("##position", posinp);
                contextbody->position = {posinp[0], posinp[1]}; // this reduces from double to float <:(

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("velocity");
                ImGui::TableNextColumn();
                float velinp[2] = {(float)contextbody->velocity.x, (float)contextbody->velocity.y};
                ImGui::SetNextItemWidth(100);
                ImGui::InputFloat2("##velocity", velinp);
                contextbody->velocity = {velinp[0], velinp[1]}; // this reduces from double to float <:(

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("acceleration");
                ImGui::TableNextColumn();
                float accinp[2] = {(float)contextbody->acceleration.x, (float)contextbody->acceleration.y};
                ImGui::SetNextItemWidth(100);
                ImGui::InputFloat2("##acceleration", accinp);
                contextbody->acceleration = {accinp[0], accinp[1]}; // this reduces from double to float <:(


                ImGui::EndTable();

            }


            togglecontext = true;
            ImGui::EndPopup();
        }
        else{
            togglecontext = false;
            contextbody = nullptr;
            can_place = true;
            //std::cout << "hi" << std::endl;
        }

        ImGui::SetNextWindowPos(ImVec2(0.0,window.getSize().y-window_height/10.0));
        ImGui::SetNextWindowSize(ImVec2(window.getSize().x,window_height/10.0));
        //ImGui::SetNextWindowContentSize(ImVec2(window_width,window_height/10.0));

        ImGuiWindowFlags flags=0;
        flags |= ImGuiWindowFlags_NoTitleBar;
        flags |= ImGuiWindowFlags_NoResize;
        flags |= ImGuiWindowFlags_NoMove;
        flags |= ImGuiWindowFlags_NoScrollbar;


        ImGui::Begin("Hello, world!", NULL, flags);

        if(ImGui::Button("Settings", ImVec2(window_height/10.0 - 15.0, window_height/10.0 - 15.0))){
            ImGui::OpenPopup("settings");
            can_place = false;
        }

        if(ImGui::BeginPopupModal("settings", NULL, ImGuiWindowFlags_Tooltip)){
            if(ImGui::Button("close")){
                //ImGui::
                ImGui::CloseCurrentPopup();
                can_place = true;
            }
            ImGui::EndPopup();
        }
        //ImGui::Mod;

        ImGui::SameLine();

        ImGui::BeginGroup();
        /*
        if(ImGui::Button("Pause", ImVec2(window_height/20.0 -15.0, window_height/20.0 -15.0))){
            paused = !paused;
        }
         */
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
            ImGui::SliderFloat("timescale",&timescale, 0.001, 2.0);
            if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
                ImGui::BeginTooltip();
                ImGui::Text("Changes the speed of the simulation. Higher->faster");
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
        //ImGui::SetWindowSize(ImVec2(window_width,window_height/10.0));
        ImGui::SetWindowPos(ImVec2(0.0,0.0));
        ImGui::Text("FPS %.2f", 1/dt.asSeconds() );
        ImGui::Text("Entity Count %i", (int)space.bodies.size());
        ImGui::Text("Iterations: %i", iterations);
        ImGui::Text("Mouse pos: (%i, %i)", (int)window.mapPixelToCoords(sf::Mouse::getPosition()).x, (int)window.mapPixelToCoords(sf::Mouse::getPosition()).y);
        ImGui::Text("Current brush: (%s)", selected_brush.c_str());
        ImGui::End();

        //ImGui::EndFrame();
        ImGui::ShowDemoWindow();

        window.clear();

        //space.draw(window);



        //sf::Image img;

        sf::Texture tex;
        tex.create(1920,1080);
        sf::Sprite sprite;
        sprite.setTexture(tex);

        //std::vector<sf::Uint8> pix(1920*1080*4, 0);


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
                //body.shape.setFillColor(body.convert_to_rgb(0,maxvel,vel));
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

                //std::cout << (int)body.shape.getFillColor().r << " " << (int)body.shape.getFillColor().g << " " << (int)body.shape.getFillColor().b << std::endl;
                for (int i = 0; i < scale; i++) {
                    for (int j = 0; j < scale; j++) {
                        pix[4 * (((gridy + i) * 1920) + gridx + j) + 0] = body.shape.getFillColor().r;
                        pix[4 * (((gridy + i) * 1920) + gridx + j) + 1] = body.shape.getFillColor().g;
                        pix[4 * (((gridy + i) * 1920) + gridx + j) + 2] = body.shape.getFillColor().b;
                        pix[4 * (((gridy + i) * 1920) + gridx + j) + 3] = std::min(255,(int)pix[4 * (((gridy + i) * 1920) + gridx + j) + 3]+ (int)(255 * brightness));
                    }
                }
                //for(int i = 0; i < 4; i++)
                //   pix[4*((gridy*1920) + gridx) + i] = std::min(pix[4*((gridy*1920) + gridx) + i]+255, 255);
            }

            //img.create(1920,1080,pix.data());

            //tex.loadFromImage(img);
            tex.update(pix);

            sprite.setOrigin(960, 540);
            sprite.setPosition(view.getCenter().x, view.getCenter().y);
            sprite.setScale((float) view.getSize().x / 1920.0f, (float) view.getSize().y / 1080.0f);

            window.draw(sprite);//, &shader);
        }
        else {
            for(auto& body : space.bodies) {
                if (&body == contextbody) {
                    body.shape.setOutlineColor(sf::Color::White);
                    body.shape.setOutlineThickness(body.shape.getRadius() / 10.0);
                } else {
                    body.shape.setOutlineThickness(0.0);
                }
            }
            space.draw(window);
        }


        //auto start = std::chrono::high_resolution_clock::now();
        //std::chrono::high_resolution_clock::time_point timeA, timeB;
        space.updateTree(tree_brightness);
        space.updateForces(false);
        space.updateBodies(dt.asMicroseconds() * timescale * paused);
        //space.draw(window);
        if(render_tree) space.drawTree(window);


        ImGui::SFML::Render(window);
        window.display();

        if(simple_render) delete[] pix;

        dt = deltaClock.restart();
        if(paused) iterations++;
    }

    ImGui::SFML::Shutdown();
    return 0;
}