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

#if DEBUG
    this->shape = sf::CircleShape(radius);
    this->shape.setPosition((float)position.x, (float)position.y);
    this->shape.setOrigin(radius,radius);

    // debug
    this->veldir = sf::RectangleShape(sf::Vector2f(50.0, 3.0));
    this->accdir = sf::RectangleShape(sf::Vector2f(50.0, 3.0));
    accdir.setFillColor(sf::Color(100,100,100));
#endif
}

void Body::update(sf::Int64 delta) {
    int totForce = std::min(( (sqrt(pow(acceleration.x,2) + pow(acceleration.y,2)) * mass)/ 200000000) * 255, (double)255);
    shape.setFillColor(sf::Color(100+std::min(totForce,155), 100+std::min(155,totForce), 255-totForce));
    shape.setFillColor(sf::Color(100+std::min(totForce,155), 100+std::min(155,totForce), 255-totForce));
    velocity += acceleration * (delta/ 1000000.0);
    position += velocity * (delta/ 1000000.0);
}

void Body::draw(sf::RenderWindow &window) {
    shape.setPosition((float)position.x, (float)position.y);
    //shape.setFillColor(sf::Color(  std::min(255, (int)sqrt(velocity.x*velocity.x + velocity.y*velocity.y)) ,   255- std::min(255, (int)sqrt(velocity.x*velocity.x + velocity.y*velocity.y))  ,0));

    //int totForce = std::min((sqrt(pow(acceleration.x,2) + pow(acceleration.y,2))/ 10) * 255, (double)255);
    //shape.setFillColor(sf::Color(totForce, totForce, 255-totForce));


    window.draw(shape);
#if DEBUG
    if(debug) {
        veldir.setPosition((float) position.x, (float) position.y);
        //veldir.setSize(sf::Vector2f(sqrt(velocity.x*velocity.x + velocity.y*velocity.y), 3.0));
        double vrot = atan2(velocity.y, velocity.x);
        veldir.setRotation(vrot * (180 / M_PI));

        accdir.setPosition((float) position.x, (float) position.y);
        //veldir.setSize(sf::Vector2f(sqrt(acceleration.x*acceleration.x + acceleration.y*acceleration.y), 3.0));
        double arot = atan2(acceleration.y, acceleration.x);
        accdir.setRotation(arot * (180 / M_PI));
    }

    if(debug) {
        window.draw(veldir);
        window.draw(accdir);
    }
#endif
}

