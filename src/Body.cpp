//
// Created by ron0 on 19/07/23.
//

#include "Body.h"

Body::Body(int mass, int radius, sf::Vector2f position, sf::Vector2f velocity) {
    this->mass = mass;
    this->radius = radius;
    this->position = position;


    this->velocity = velocity;
    acceleration = sf::Vector2f(0.0f, 0.0f);

    this->shape = sf::CircleShape(radius);
    this->shape.setPosition(position);
}

void Body::update() {
    velocity += acceleration;
    position += velocity;
}

void Body::draw(sf::RenderWindow &window) {
    shape.setPosition(position);
    window.draw(shape);
}

