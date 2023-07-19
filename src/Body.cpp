//
// Created by ron0 on 19/07/23.
//

#include "Body.h"
#include <algorithm>
#include <math.h>

Body::Body(float mass, float radius, sf::Vector2<double> position, sf::Vector2<double> velocity) {
    this->mass = mass;
    this->radius = radius;
    this->position = position;


    this->velocity = velocity;
    acceleration = sf::Vector2<double>(0.0, 0.0);

    this->shape = sf::CircleShape(radius);
    this->shape.setPosition((float)position.x, (float)position.y);
    this->shape.setOrigin(radius,radius);
}

void Body::update(sf::Int64 delta) {
    velocity += acceleration * (delta/ 1000000.0);
    position += velocity * (delta/ 1000000.0);
}

void Body::draw(sf::RenderWindow &window) {
    shape.setPosition((float)position.x, (float)position.y);
    shape.setFillColor(sf::Color(  std::min(255, (int)sqrt(velocity.x*velocity.x + velocity.y*velocity.y)) ,   255- std::min(255, (int)sqrt(velocity.x*velocity.x + velocity.y*velocity.y))  ,0));
    window.draw(shape);
}

