#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include "Simulator.h"
#include <thread>
#include <chrono>


int main()
{
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "OrbitSim");
    window.setFramerateLimit(60); // for physics
    int sizex = 1920*1;
    int sizey = 1080*1;
    sf::View view(sf::FloatRect(-sizex/2.0f, -sizey/2.0f, sizex, sizey));
    window.setView(view);

    Simulator space(1000000);

    /*
    space.addBody(Body(10000,100,sf::Vector2<double>(0, 0),sf::Vector2<double>(0,400) ));
    space.bodies.back().shape.setFillColor(sf::Color::Green);

    space.addBody(Body(100000000,100,sf::Vector2<double>(500, 0) ));
    space.bodies.back().shape.setFillColor(sf::Color::Green);
     */
    for(int i = 0; i < 1015; i++) {
        //double dx = (((double)rand()/ RAND_MAX) * 28284) -14142;
        //double dy = (((double)rand()/ RAND_MAX) * 28284) -14142;

        double rnd = rand();
        double r = 20000 * sqrt( pow((double)rand()/RAND_MAX, 2));
        double theta = ((double)rnd/RAND_MAX) * 2 * M_PI;
        double x = r * cos(theta);
        double y = r * sin(theta);

        //double x =  cos(((double)rnd/ RAND_MAX) * 2*M_PI) * 20000 + dx;
        //double y =  sin(((double)rnd/ RAND_MAX) * 2*M_PI) * 20000 + dy;

        //std::cout << x << " " << y << std::endl;
        //std::cout << (1-(sqrt(x*x + y*y)/56568)) << std::endl;

        double rnd2 = (((double)rand()/ RAND_MAX)*10);
        double velx = -cos(M_PI*0.5 - ( ((double)rnd/ RAND_MAX) * 2*M_PI )) *  20 * (2- r/20000) * 1;
        double vely =  sin(M_PI*0.5 - ( ((double)rnd/ RAND_MAX) * 2*M_PI )) *  20 * (2- r/20000) * 1;
        double mass = rnd2;

        space.addBody(Body(100,10 * mass,sf::Vector2<double>(x, y), sf::Vector2<double>(velx*10,vely*10)));
        //space.bodies.back().shape.setFillColor(sf::Color(255,255,255,40));
        //space.bodies.back().shape.setFillColor()
    }
    space.addBody(Body(1000000,10,sf::Vector2<double>(0, 0)));
    space.bodies.back().shape.setFillColor(sf::Color::Green);



    /*
    for(int i = -50000; i < 50000; i+=1000){
        for(int j = -50000; j < 50000; j+=1000){

            double rnd = (((double)rand()/ RAND_MAX) * 10000);

            double rnd2 = (((double)rand()/ RAND_MAX)*1000)-500;
            double rnd3 = (((double)rand()/ RAND_MAX)*1000)-500;
            space.addBody(Body(rnd, 100 * rnd/10000, sf::Vector2<double>(i,j)));
            //space.bodies.back().shape.setFillColor(sf::Color(255,255,255,40));
        }
    }
    */


    /*
    for(int i = 0; i < 50005; i++) {
        double dx = (((double)rand()/ RAND_MAX) * 5000) -2500;
        double dy = (((double)rand()/ RAND_MAX) * 5000) -2500;

        int rnd = rand();
        double x =  cos(((double)rnd/ RAND_MAX) * 2*M_PI) * 20000 + dx;
        double y =  sin(((double)rnd/ RAND_MAX) * 2*M_PI) * 20000 + dy;

        double rnd2 = (((double)rand()/ RAND_MAX)*10);
        double velx = -cos(M_PI*0.5 - ( ((double)rnd/ RAND_MAX) * 2*M_PI )) * rnd2;
        double vely =  sin(M_PI*0.5 - ( ((double)rnd/ RAND_MAX) * 2*M_PI )) * rnd2;
        double mass = rnd2;

        space.addBody(Body(mass,100 * mass,sf::Vector2<double>(x, y), sf::Vector2<double>(velx*10,vely*10)));
        space.bodies.back().shape.setFillColor(sf::Color(255,255,255,40));
    }
    space.addBody(Body(10000000,100,sf::Vector2<double>(0, 0)));
    space.bodies.back().shape.setFillColor(sf::Color::Green);

    /*
    for(int i = 0; i < 25000; i++) {
        double dx = (((double)rand()/ RAND_MAX) * 200) -100;
        double dy = (((double)rand()/ RAND_MAX) * 200) -100;

        int rnd = rand();
        double x =  cos(((double)rnd/ RAND_MAX) * 2*M_PI) * 2000 + dx;
        double y =  sin(((double)rnd/ RAND_MAX) * 2*M_PI) * 2000 + dy;

        double rnd2 = (((double)rand()/ RAND_MAX)*10);
        double velx = -cos(M_PI*0.5 - ( ((double)rnd/ RAND_MAX) * 2*M_PI )) * rnd2;
        double vely =  sin(M_PI*0.5 - ( ((double)rnd/ RAND_MAX) * 2*M_PI )) * rnd2;
        double mass = rnd2;

        velx += -10;
        vely += -50;

        space.addBody(Body(mass,100 * mass,sf::Vector2<double>(x+20000, y-20000), sf::Vector2<double>(velx*10,vely*10)));
        space.bodies.back().shape.setFillColor(sf::Color(255,255,255,40));
    }*/
    //space.addBody(Body(100000000,100,sf::Vector2<double>(20000, -10000), sf::Vector2<double>(-100,20)));
    //space.bodies.back().shape.setFillColor(sf::Color::Green);


    /*
    for(int i = 0; i < 25005; i++) {
        double dx = (((double)rand()/ RAND_MAX) * 200) -100;
        double dy = (((double)rand()/ RAND_MAX) * 200) -100;

        int rnd = rand();
        double x =  cos(((double)rnd/ RAND_MAX) * 2*M_PI) * 2000 + dx;
        double y =  sin(((double)rnd/ RAND_MAX) * 2*M_PI) * 2000 + dy;

        double rnd2 = (((double)rand()/ RAND_MAX)*10);
        double velx = -cos(M_PI*0.5 - ( ((double)rnd/ RAND_MAX) * 2*M_PI )) * rnd2;
        double vely =  sin(M_PI*0.5 - ( ((double)rnd/ RAND_MAX) * 2*M_PI )) * rnd2;
        double mass = rnd2;

        velx += 10;
        vely += 50;

        space.addBody(Body(mass,100 * mass,sf::Vector2<double>(x-20000, y+20000), sf::Vector2<double>(velx*10,vely*10)));
        space.bodies.back().shape.setFillColor(sf::Color(255,255,255,40));
    }*/
    //space.addBody(Body(100000000,100,sf::Vector2<double>(-20000, +10000), sf::Vector2<double>(100,-20)));
    //space.bodies.back().shape.setFillColor(sf::Color::Green);


    //space.addBody(Body(10000000,100,sf::Vector2<double>(-5000, -1000),    sf::Vector2<double>(40, 100)));
    //space.bodies.back().debug = true;
    //space.addBody(Body(10000000,100,sf::Vector2<double>(5000, 1000), sf::Vector2<double>(-40,-100)));
    //space.bodies.back().debug = true;

    /*
    for(int i = 0; i < 1002; i++)
    {
       // std::cout << space.bodies[i].position.x << " " << space.bodies[i].position.y << std::endl;
        for(int j = i+1; j < 1002; j++)  {
            if(space.bodies[i].position.x == space.bodies[j].position.x and space.bodies[i].position.y == space.bodies[j].position.y)
                std::cout << "WHAT\n" << std::endl;
        }

    }
     */


    /*
    space.addBody(Body(1,10,sf::Vector2<double>(-100, -100)));
    space.addBody(Body(1,10,sf::Vector2<double>(100, 100)));
    space.addBody(Body(1,10,sf::Vector2<double>(100, 75)));
    space.addBody(Body(1,10,sf::Vector2<double>(100, -175)));
    space.addBody(Body(1,10,sf::Vector2<double>(-100, -75)));
    */



    sf::Clock deltaClock;
    sf::Time dt;
    int iterations = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))  window.close();
            if (event.type == sf::Event::Closed)                       window.close();


            /*if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                view.setSize(view.getSize().x * 0.8, view.getSize().y * 0.8);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                view.setSize(view.getSize().x * 1.25, view.getSize().y * 1.25);
            window.setView(view);*/
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            view.setSize(view.getSize().x * (1 - 1 * dt.asSeconds()), view.getSize().y * (1 - 1 * dt.asSeconds()));
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            view.setSize(view.getSize().x * (1 + 1 * dt.asSeconds()), view.getSize().y * (1 + 1 * dt.asSeconds()));
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            view.setCenter(view.getCenter().x, view.getCenter().y - 5000*dt.asSeconds());
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            view.setCenter(view.getCenter().x - 5000*dt.asSeconds(), view.getCenter().y);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            view.setCenter(view.getCenter().x, view.getCenter().y + 5000*dt.asSeconds());
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            view.setCenter(view.getCenter().x + 5000*dt.asSeconds(), view.getCenter().y);
        window.setView(view);

        window.clear();
        //space.draw(window);
        sf::Image img;
        sf::Uint8 pix[1920*1080*4] = {0};


        sf::Vector2f origin = sf::Vector2f(view.getCenter().x-view.getSize().x/2, view.getCenter().y-view.getSize().y/2);
        sf::Vector2f size = sf::Vector2f(view.getSize().x, view.getSize().y);

        for(auto & body  : space.bodies){
            sf::Vector2<double> pos = body.position;
            if(pos.x < origin.x or pos.x > origin.x + size.x) continue;
            if(pos.y < origin.y or pos.y > origin.y + size.y) continue;

            int gridx = (pos.x - origin.x)/(size.x/1920);
            int gridy = (pos.y - origin.y)/(size.y/1080);

            for(int i = 0; i < 4; i++)
                pix[4*((gridy*1920) + gridx) + i] = std::min(pix[4*((gridy*1920) + gridx) + i]+255, 255);
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
        if(iterations % 1 == 0) {


            /*
            auto f = [&space](){
                space.updateTree();
                space.updateForces();
            };
            std::thread t(f);
            //t.detach();
            t.join();
            */

            //space.updateTree();
            //timeA = std::chrono::high_resolution_clock::now();
            //space.updateForces();
            //timeB = std::chrono::high_resolution_clock::now();

            //auto treetime = std::chrono::duration_cast<std::chrono::microseconds>(timeA - start);
            //auto forcetime = std::chrono::duration_cast<std::chrono::microseconds>(timeB - timeA);
            //std::cout << treetime.count() << " " << forcetime.count() << std::endl;
        }

        space.updateTree();
        space.updateForces();
        space.draw(window);
        space.updateBodies(dt.asMicroseconds());

        window.display();

        //view.setCenter(space.bodies.back().position.x, space.bodies.back().position.y);
        //window.setView(view);


        dt = deltaClock.restart();
        iterations++;

        std::cout << dt.asSeconds() << std::endl;
        //std::cout << space.bodies.back().acceleration.x << " " << space.bodies.back().acceleration.y << std::endl;
    }

    return 0;
}