//
// Created by ron0 on 19/07/23.
//

#include "Simulator.h"
//#include <iostream>

Simulator::Simulator() {

}

void Simulator::addBody(Body body) {
    bodies.push_back(body);
}

// returns the length of a vector squared
float squarelen(sf::Vector2f a){
    return a.x*a.x + a.y*a.y;
}

void Simulator::update() {
    for(auto & bodyA : bodies) {
        bodyA.acceleration = sf::Vector2f(0.0f, 0.0f);
        for(auto & bodyB : bodies) {
            if(&bodyA == &bodyB) continue;
            float magnitude = (bodyA.mass * bodyB.mass)/squarelen(bodyB.position-bodyA.position);
            sf::Vector2f force = bodyB.position-bodyA.position;
            force.x *= magnitude/ sqrt(squarelen(bodyB.position-bodyA.position));
            force.y *= magnitude/ sqrt(squarelen(bodyB.position-bodyA.position));

            sf::Vector2f accel(force.x/bodyA.mass, force.y/bodyA.mass);
            bodyA.acceleration += accel;
        }
        //std::cout << bodyA.position.x << " " << bodyA.position.y << std::endl;
    }
    for(auto & body : bodies) body.update();
}

void Simulator::draw(sf::RenderWindow& window) {
    for(auto & body: bodies) {
        body.draw(window);
    }
}
