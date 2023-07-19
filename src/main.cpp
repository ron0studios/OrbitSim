#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include "Simulator.h"

int main()
{
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "OrbitSim");
    window.setFramerateLimit(60); // for physics
    int sizex = 1920*3;
    int sizey = 1080*3;
    sf::View view(sf::FloatRect(-sizex/2.0f, -sizey/2.0f, sizex, sizey));
    window.setView(view);

    Simulator space;
    for(int i = 0; i < 1000; i++) {
        int dx = (((double)rand()/ RAND_MAX) * 200) -100;
        int dy = (((double)rand()/ RAND_MAX) * 140) -70;

        int rnd = rand();
        int x =  cos(((double)rnd/ RAND_MAX) * 2*M_PI) * 300 + dx;
        int y =  sin(((double)rnd/ RAND_MAX) * 2*M_PI) * 300 + dy;

        double rnd2 = (((double)rand()/ RAND_MAX)*10);
        double velx = -cos(M_PI*0.5 - ( ((double)rnd/ RAND_MAX) * 2*M_PI )) * rnd2;
        double vely =  sin(M_PI*0.5 - ( ((double)rnd/ RAND_MAX) * 2*M_PI )) * rnd2;
        double mass = 1;

        space.addBody(Body(mass,10,sf::Vector2<double>(x, y), sf::Vector2<double>(velx*10,vely*10)));
    }
    space.addBody(Body(1000,10,sf::Vector2<double>(0, 0)));
    space.bodies.back().shape.setFillColor(sf::Color::Green);


    for(auto i : space.bodies) {
        std::cout << i.mass << " " << i.mass << std::endl;
    }

    sf::Clock deltaClock;
    sf::Time dt;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))  window.close();
            if (event.type == sf::Event::Closed)                       window.close();
        }


        window.clear();
        space.draw(window);
        space.update(dt.asMicroseconds());
        window.display();

        for(auto i : space.bodies) {
            if (isnan(i.position.x)) {
                break;
            }
            //std::cout << i.position.x << " " << i.position.y << std::endl;
        }

        dt = deltaClock.restart();
    }

    return 0;
}