//
// Created by ron0 on 19/07/23.
//

#include "Simulator.h"
#include <iostream>

Simulator::Simulator(double bounds) {
    this->bounds = bounds;
    this->root = QuadTreeNode(sf::Vector2<double>(0.0,0.0), 0.0, bounds*2.0);

    boundbox = sf::RectangleShape(sf::Vector2f(bounds*2,bounds*2));
    boundbox.setPosition(sf::Vector2f(-bounds, -bounds));
    boundbox.setFillColor(sf::Color::Transparent);
    boundbox.setOutlineThickness(100);
}

void Simulator::addBody(Body body) {
    bodies.push_back(body);
}

// returns the length of a vector squared
double Simulator::squarelen(sf::Vector2<double> a){
    if(a.x == a.y and a.x == 0)
        a.x = a.y = 0.1;

    return a.x*a.x + a.y*a.y;
}


void Simulator::genQuadTree(){
    int id = 0;
    for(auto & body : bodies) {
        id++;
        sf::Vector2<double> bpos = body.position;

        if(abs(bpos.x) > bounds or abs(bpos.y) > bounds) {
            continue;
        }

        QuadTreeNode* top = &root;

        while(true) {
            // deduce quadrant
            int part = top->getQuadrantIdx(bpos);
            printf("part: %i\n", part);
            if(top->getChildFromIdx(part) != nullptr)
            {
                top->getChildFromIdx(part)->mass += body.mass;
                top = top->getChildFromIdx(part);
            }
            else
            {
                sf::Vector2<double> pos = top->getQuadrantCenter(top->getChildFromIdx(part));
                top->setQuadrant(part, new QuadTreeNode(pos, body.mass, top->width/2));
                break;
            }
        }
    }
}

void Simulator::update(sf::Int64 delta) {

    genQuadTree();
/*
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
    */
}

void Simulator::draw(sf::RenderWindow& window) {
    //window.draw(boundbox);
    root.draw(window);
    for(auto & body: bodies) {
        body.draw(window);
    }
}

