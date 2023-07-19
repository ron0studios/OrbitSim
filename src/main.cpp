#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include "Simulator.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "OrbitSim");
    window.setFramerateLimit(60); // for physics
    int sizex = 1920*10;
    int sizey = 1080*10;
    sf::View view(sf::FloatRect(-sizex/2.0f, -sizey/2.0f, sizex, sizey));
    window.setView(view);

    Simulator space;
    for(int i = 0; i < 100; i++) {
        int randx = (((float)rand()/ RAND_MAX) * 1000) + 100;
        int randy = (((float)rand()/ RAND_MAX) * 700) + 70;
        float velx =  (((float)rand()/ RAND_MAX) * 10)-5;
        float vely =  (((float)rand()/ RAND_MAX) * 10)-5;
        float mass = (((float)rand()/ RAND_MAX) * 20);

        space.addBody(Body(1,10,sf::Vector2f(randx, randy), sf::Vector2f(velx,vely)));
    }
    space.addBody(Body(5000,1,sf::Vector2f(500, 300)));

    /*for(auto i : space.bodies) {
        std::cout << i.position.x << " " << i.position.y << std::endl;
    }*/

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
        space.update();
        window.display();

    }

    return 0;
}