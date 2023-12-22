#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include "Simulator.h"
#include <thread>
#include <chrono>
#include "imgui-SFML.h"
#include "imgui.h"

void addGalaxy(Simulator& sim, double starnum, double starmass, double c_mass, double rad, double rotspeed, double cx, double cy, double c_velx, double c_vely, double radial=0, double massvar = 10.0){
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
    int scale = 4;
    float brightness = 1;
    float tree_brightness = 0.04;

    //addGalaxy(space, 10000, 1000, 1000, 1000, 0.2, -00000, -00000, -0, 000,.0, 1.0);
    //addGalaxy(space, 10000, 1000, 1000, 10000, 0, -00000, -00000, -0, 000,.0, 1.0);

    addGalaxy(space, 100, 1000, 1000, 100, 1, -00000, -00000, 0, 000,0.0, 1.0);
    addGalaxy(space, 100, 1000, 1000, 100, 1, -1000, -1000, 1000, 000,0.0, 1.0);
    addGalaxy(space, 100, 1000, 1000, 100, 1, 1000, 1000, 0, 000,0.0, 1.0);
    addGalaxy(space, 100, 1000, 1000, 100, 1, 1000, -1000, 0, 000,0.0, 1.0);
    addGalaxy(space, 100, 1000, 1000, 100, 1, -1000, 1000, 0, 000,0.0, 1.0);

    addGalaxy(space, 100, 1000, 1000, 100, 1, 10000, -00000, 0, 000,0.0, 1.0);
    addGalaxy(space, 100, 1000, 1000, 100, 1, 10000-1000, -1000, 1000, 000,0.0, 1.0);
    addGalaxy(space, 100, 1000, 1000, 100, 1, 10000+1000, 1000, 0, 000,0.0, 1.0);
    addGalaxy(space, 100, 1000, 1000, 100, 1, 10000+1000, -1000, 0, 000,0.0, 1.0);
    addGalaxy(space, 100, 1000, 1000, 100, 1, 10000-1000, 1000, 0, 000,0.0, 1.0);



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

        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);
            //if(event.type == sf::Event::GainedFocus) focus = true;
            //if(event.type == sf::Event::LostFocus) focus = false;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))  window.close();
            if (event.type == sf::Event::Closed)                       window.close();
            if(event.type == sf::Event::KeyReleased and event.key.code == sf::Keyboard::Space)
                paused = !paused;


            /*
            if(event.type == sf::Event::MouseButtonReleased and event.key.code == sf::Mouse::Left){
                sf::Vector2<double> pos = (sf::Vector2<double>)window.mapPixelToCoords(sf::Mouse::getPosition());
                if((float)sf::Mouse::getPosition().y/window.getSize().y > 0.9) continue;
                //addGalaxy(space, 100, 10000, 0, 1000, 0, pos.x, pos.y, -0, 000,.0, 1.0);
                addGalaxy(space, 0, 10000, 100000000000, 1000, 0, pos.x, pos.y, -0, 000,.0, 1.0);
            }
             */


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
        }

        if(ImGui::BeginPopupModal("settings", NULL, ImGuiWindowFlags_Tooltip)){
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
            ImGui::Checkbox("Render arrows?", &use_colors);
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
                ImGui::EndTooltip();
            }
            ImGui::SliderFloat("star brightness", &brightness, 0.1, 1);
            if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
                ImGui::BeginTooltip();
                ImGui::Text("sets the brightness of simple rendering stars by modifying its alpha value");
                ImGui::EndTooltip();
            }
            ImGui::SliderFloat("tree brightness",&tree_brightness, 0.01, 1.0);
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
        //ImGui::SetNextItemWidth(window_width/5.0);

        ImGui::EndGroup();

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
        ImGui::End();

        //ImGui::EndFrame();
        //ImGui::ShowDemoWindow();

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


            for (auto &body: space.bodies) {
                sf::Vector2<double> pos = body.position;
                if (!use_colors) body.shape.setFillColor(sf::Color::White);
                if (pos.x < origin.x or pos.x > origin.x + size.x) continue;
                if (pos.y < origin.y or pos.y > origin.y + size.y) continue;

                int gridx = (pos.x - origin.x) / (size.x / 1920.0);
                gridx -= gridx % scale;
                int gridy = (pos.y - origin.y) / (size.y / 1080.0);
                gridy -= gridy % scale;


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

            window.draw(sprite);
        }
        else{ space.draw(window); }


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