#include <SFML/Graphics.hpp>
#include <random>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!");
    window.setFramerateLimit(60);

    sf::CircleShape shape(50.f);
    shape.setFillColor(sf::Color(10,200,150));


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }


        window.clear();
        window.draw(shape);
        window.display();

    }

    return 0;
}