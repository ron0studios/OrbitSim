#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include "Simulator.h"

int main()
{
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "OrbitSim");
    window.setFramerateLimit(60); // for physics
    int sizex = 1920*10;
    int sizey = 1080*10;
    sf::View view(sf::FloatRect(-sizex/2.0f, -sizey/2.0f, sizex, sizey));
    window.setView(view);

    Simulator space;

    for(int i = 0; i < 1000; i++) {
        double dx = (((double)rand()/ RAND_MAX) * 500) -250;
        double dy = (((double)rand()/ RAND_MAX) * 500) -250;

        int rnd = rand();
        double x =  cos(((double)rnd/ RAND_MAX) * 2*M_PI) * 3000 + dx;
        double y =  sin(((double)rnd/ RAND_MAX) * 2*M_PI) * 3000 + dy;

        double rnd2 = (((double)rand()/ RAND_MAX)*10);
        double velx = -cos(M_PI*0.5 - ( ((double)rnd/ RAND_MAX) * 2*M_PI )) * rnd2;
        double vely =  sin(M_PI*0.5 - ( ((double)rnd/ RAND_MAX) * 2*M_PI )) * rnd2;
        double mass = 1;

        space.addBody(Body(mass,10,sf::Vector2<double>(x, y), sf::Vector2<double>(velx*10,vely*10)));
    }
    space.addBody(Body(100000,100,sf::Vector2<double>(0, 0)));
    space.addBody(Body(10000000,100,sf::Vector2<double>(5000, 1000), sf::Vector2<double>(-40,-100)));

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
        space.draw(window);
        space.update(dt.asMicroseconds());
        sf::CircleShape s(1);
        s.setPosition(0,0);
        window.draw(s);

        window.display();

        //view.setCenter(space.bodies.back().position.x, space.bodies.back().position.y);
        //window.setView(view);


        dt = deltaClock.restart();
    }

    return 0;
}