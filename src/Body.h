//
// Created by ron0 on 19/07/23.
//
#include <SFML/Graphics.hpp>
#include "globals.h"

#ifndef ORBITSIM_BODY_H
#define ORBITSIM_BODY_H


class Body {
    public:
        sf::CircleShape shape;
        double mass, radius;
        sf::Vector2<double> acceleration, velocity, position;

        // debug
        bool debug = false;
        sf::RectangleShape veldir;
        sf::RectangleShape accdir;

        Body(float mass, float radius, sf::Vector2<double> position, sf::Vector2<double> velocity = sf::Vector2<double>(0.0f, 0.0f));
        void update(sf::Int64 delta, double maxForce); // updates based on acceleration and velocity
        void draw(sf::RenderWindow& window);
        static sf::Color convert_to_rgb(double min, double max, double val);
};


#endif //ORBITSIM_BODY_H
