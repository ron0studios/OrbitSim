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
        sf::Vector2<double> acceleration, velocity, position;

        Body(float mass, float radius, sf::Vector2<double> position, sf::Vector2<double> velocity = sf::Vector2<double>(0.0f, 0.0f));
        void update(sf::Int64 delta); // updates based on acceleration and velocity
        void draw(sf::RenderWindow& window);
};


#endif //ORBITSIM_BODY_H
