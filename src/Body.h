//
// Created by ron0 on 19/07/23.
//
#include <SFML/Graphics.hpp>

#ifndef ORBITSIM_BODY_H
#define ORBITSIM_BODY_H


class Body {
    public:
        sf::CircleShape shape;
        float mass, radius;
        sf::Vector2f acceleration, velocity, position;

        Body(int mass, int radius, sf::Vector2f position, sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f));
        void update(); // updates based on acceleration and velocity
        void draw(sf::RenderWindow& window);
};


#endif //ORBITSIM_BODY_H
