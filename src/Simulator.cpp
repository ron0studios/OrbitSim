//
// Created by ron0 on 19/07/23.
//

#include "Simulator.h"
#include <iostream>

Simulator::Simulator() {

}

void Simulator::addBody(Body body) {
    bodies.push_back(body);
}

// returns the length of a vector squared
double squarelen(sf::Vector2<double> a){
    if(a.x == a.y and a.x == 0)
        a.x = a.y = 0.1;

    return a.x*a.x + a.y*a.y;
}

void Simulator::update(sf::Int64 delta) {
    for(auto & bodyA : bodies) {
        bodyA.acceleration = sf::Vector2<double>(0.0, 0.0);
        for(auto & bodyB : bodies) {
            if(&bodyA == &bodyB) continue;
            double magnitude = (bodyA.mass * bodyB.mass)/( 0.1 * squarelen(bodyB.position-bodyA.position));
            //if(isnan(magnitude) or isinf(magnitude)) magnitude = 3 * pow(10,7); // 10% speed of light


            sf::Vector2<double> force = bodyB.position-bodyA.position;
            //std::cout << "\t" << force.x << " " << force.y << std::endl;
            force.x *= magnitude/ sqrt(squarelen(bodyB.position-bodyA.position));
            force.y *= magnitude/ sqrt(squarelen(bodyB.position-bodyA.position));


            sf::Vector2<double> accel(force.x/bodyA.mass, force.y/bodyA.mass);
            bodyA.acceleration += accel;

        }
        //std::cout << "\t" << bodyA.acceleration.x << " " << bodyA.acceleration.y << std::endl;
    }
    for(auto & body : bodies) body.update(delta);
}

void Simulator::draw(sf::RenderWindow& window) {
    for(auto & body: bodies) {
        body.draw(window);
    }
}
