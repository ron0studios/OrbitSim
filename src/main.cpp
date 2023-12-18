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
    int iterations = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if(event.type == sf::Event::GainedFocus) focus = true;
            if(event.type == sf::Event::LostFocus) focus = false;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))  window.close();
            if (event.type == sf::Event::Closed)                       window.close();
            if(event.type == sf::Event::KeyReleased and event.key.code == sf::Keyboard::Space)
                paused = !paused;

            /*if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                view.setSize(view.getSize().x * 0.8, view.getSize().y * 0.8);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                view.setSize(view.getSize().x * 1.25, view.getSize().y * 1.25);
            window.setView(view);*/
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
            //if(sf::Keyboard::(sf::Keyboard::Space))  paused = !paused;
            //view.setCenter(view.getCenter().x + speed*view.getSize().x*dt.asSeconds(), view.getCenter().y);
        }
        window.setView(view);


        ImGui::SFML::Update(window, deltaClock.restart());

        ImGuiWindowFlags flags;
        flags |= ImGuiWindowFlags_NoTitleBar;
        flags |= ImGuiWindowFlags_NoResize;
        flags |= ImGuiWindowFlags_NoMove;
        ImGui::Begin("Hello, world!", NULL, flags);
        ImGui::SetWindowPos(ImVec2(0.0,0.9*window_height));
        ImGui::SetWindowSize(ImVec2(window_width,window_height/10.0));

        if(ImGui::Button("Pause")){
            paused = !paused;
        }

        ImGui::SliderFloat("timescale",&timescale, 0.001, 2.0);
        ImGui::End();

        //ImGui::ShowDemoWindow();

        window.clear();
        //space.draw(window);
        sf::Image img;
        int scale = 4;
        float brightness = 0.5;
        sf::Uint8 pix[1920*1080*4] = {0};


        sf::Vector2f origin = sf::Vector2f(view.getCenter().x-view.getSize().x/2, view.getCenter().y-view.getSize().y/2);
        sf::Vector2f size = sf::Vector2f(view.getSize().x, view.getSize().y);


        for(auto & body  : space.bodies){
            sf::Vector2<double> pos = body.position;
            if(pos.x < origin.x or pos.x > origin.x + size.x) continue;
            if(pos.y < origin.y or pos.y > origin.y + size.y) continue;

            int gridx = (pos.x - origin.x)/(size.x/1920); gridx -= gridx % scale;
            int gridy = (pos.y - origin.y)/(size.y/1080); gridy -= gridy % scale;


            //std::cout << (int)body.shape.getFillColor().r << " " << (int)body.shape.getFillColor().g << " " << (int)body.shape.getFillColor().b << std::endl;
            for(int i = 0; i < scale; i++){
                for(int j = 0; j < scale; j++){
                    pix[4*(((gridy+i)*1920) + gridx+j) + 0] = body.shape.getFillColor().r;
                    pix[4*(((gridy+i)*1920) + gridx+j) + 1] = body.shape.getFillColor().g;
                    pix[4*(((gridy+i)*1920) + gridx+j) + 2] = body.shape.getFillColor().b;
                    pix[4*(((gridy+i)*1920) + gridx+j) + 3] += (int)((255*scale*brightness)/scale);
                }
            }
            //for(int i = 0; i < 4; i++)
             //   pix[4*((gridy*1920) + gridx) + i] = std::min(pix[4*((gridy*1920) + gridx) + i]+255, 255);
        }

        img.create(1920,1080,pix);

        sf::Texture tex;
        tex.loadFromImage(img);

        sf::Sprite sprite;
        sprite.setTexture(tex);
        sprite.setOrigin(960, 540);
        sprite.setPosition(view.getCenter().x, view.getCenter().y);
        sprite.setScale((float)view.getSize().x/1920.0f, (float)view.getSize().y/1080.0f);

        window.draw(sprite);

        //auto start = std::chrono::high_resolution_clock::now();
        //std::chrono::high_resolution_clock::time_point timeA, timeB;
        space.updateTree();
        space.updateForces(false);
        space.updateBodies(dt.asMicroseconds() * timescale * paused);
        //space.draw(window);
        //space.drawTree(window);


        if(iterations % 100 == 0) {



            /*
            auto f = [&space, dt](){
                auto start = std::chrono::high_resolution_clock::now();
                space.updateTree();
                space.updateForces(false);
                space.updateBodies(dt.asMicroseconds());
                auto timeA = std::chrono::high_resolution_clock::now();
                auto forcetime = std::chrono::duration_cast<std::chrono::microseconds>(timeA - start);
                std::cout << forcetime.count()/1000000.0 << std::endl;
            };
            std::thread t(f);
            t.detach();
            //t.join();
             */

            //space.updateTree();
            //timeA = std::chrono::high_resolution_clock::now();
            //space.updateForces();
            //timeB = std::chrono::high_resolution_clock::now();

            //auto treetime = std::chrono::duration_cast<std::chrono::microseconds>(timeA - start);
            //auto forcetime = std::chrono::duration_cast<std::chrono::microseconds>(timeB - timeA);
            //std::cout << treetime.count() << " " << forcetime.count() << std::endl;
        }


        //space.updateTree();
        //space.updateForces();
        //space.draw(window);
        //space.drawTree(window);
        //space.updateBodies(dt.asMicroseconds());

        ImGui::SFML::Render(window);
        window.display();

        //view.setCenter(space.bodies.back().position.x, space.bodies.back().position.y);
        //window.setView(view);


        dt = deltaClock.restart();
        iterations++;

        std::cout << dt.asSeconds() << "\t\t" << view.getSize().x << std::endl;
        //std::cout << space.bodies.back().acceleration.x << " " << space.bodies.back().acceleration.y << std::endl;
    }

    return 0;
}