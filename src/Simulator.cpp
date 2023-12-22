//
// Created by ron0 on 19/07/23.
//

#include "Simulator.h"
#include <iostream>
#include <stack>
#include <thread>
#include <cfloat>
#include "ctpl.h"



Simulator::Simulator(double bounds) {
    this->bounds = bounds;

    boundbox = sf::RectangleShape(sf::Vector2f(bounds*2,bounds*2));
    boundbox.setPosition(sf::Vector2f(-bounds, -bounds));
    boundbox.setFillColor(sf::Color::Transparent);
    boundbox.setOutlineThickness(100);
}

void Simulator::addBody(Body body) {
    bodies.push_back(body);
}


void Simulator::updateTree(float brightness) {
    std::lock_guard<std::mutex> guard(mut);

    tree = QuadTree(this->bounds, &bodies, brightness);

    //delQuadTree(&root);
    //genQuadTree();
}


void Simulator::updateForces(bool bruteForce) {
    std::lock_guard<std::mutex> guard(mut);
    maxForce = -1;
    //std::cout << "loop\n" << std::endl;


    if(bruteForce) {


        int num_threads = 16;
        std::vector<std::thread> threads(num_threads);

        for(int i = 0; i < num_threads; i++){
            threads[i] = std::thread([this](int i, int n){

                for(int j = i*n; j < ((i+1)*n) and j < bodies.size(); j++) {
                    for (auto &bodyB: bodies) {
                        if (&bodies[j] == &bodyB) continue;

                        bodies[j].acceleration +=
                                tree.forcePair(bodies[j].mass, bodyB.mass, bodies[j].position, bodyB.position) / bodies[j].mass;

                    }
                }

            },i, std::ceil((double)bodies.size()/num_threads));
        }


        for(int i = 0; i < num_threads; i++)
            threads[i].join();






        /*
        for (auto &bodyA: bodies) {
            bodyA.acceleration = sf::Vector2<double>(0.0, 0.0);
            for (auto &bodyB: bodies) {
                if (&bodyA == &bodyB) continue;

                bodyA.acceleration +=
                        tree.forcePair(bodyA.mass, bodyB.mass, bodyA.position, bodyB.position) / bodyA.mass;

            }
            //std::cout << "\t" << bodyA.acceleration.x << " " << bodyA.acceleration.y << std::endl;
        }
         */
        return;
    }

    /*
    for(auto &body : bodies) {
        tree.updateForce(&body, 0.5);
    }
     */


    /*
    for(int j = 0; j < bodies.size(); j++) {
        tree.updateForce(&bodies[j], 0.5);
        //maxForce = std::max(maxForce, bodies[j].mass * std::sqrt(std::pow(bodies[j].acceleration.x,2) + std::pow(bodies[j].acceleration.y,2)));
        maxForce = std::max(maxForce, std::sqrt(std::pow(bodies[j].velocity.x,2) + std::pow(bodies[j].velocity.y,2)));
    }
     */

    int num_threads = 16;
    std::vector<std::thread> threads(num_threads);

    for(int i = 0; i < num_threads; i++){
        threads[i] = std::thread([this](int i, int n){

            for(int j = i*n; j < ((i+1)*n) and j < bodies.size(); j++) {
                tree.updateForce(&bodies[j], 0.5);
                //maxForce = std::max(maxForce, bodies[j].mass * std::sqrt(std::pow(bodies[j].acceleration.x,2) + std::pow(bodies[j].acceleration.y,2)));
                maxForce = std::max(maxForce, std::sqrt(std::pow(bodies[j].velocity.x,2) + std::pow(bodies[j].velocity.y,2)));
            }

                //calcForce(bodies[j]);
        },i, std::ceil((double)bodies.size()/num_threads));
    }


    for(int i = 0; i < num_threads; i++)
        threads[i].join();



    /*
    for(auto &body : bodies) {
        calcForce(body);
    }
     */


    /*
    int num_threads = 16;
    std::vector<std::thread> threads(num_threads);

    for(int i = 0; i < num_threads; i++){
        threads[i] = std::thread([this](int i, int n){
                for(int j = i*n; j < ((i+1)*n); j++)
                    calcForce(bodies[j]);
            },i, bodies.size()/num_threads);
    }

    for(int i = 0; i < num_threads; i++)
        threads[i].join();

        */





    //for(auto & body : bodies) body.update(delta);
}

void Simulator::draw(sf::RenderWindow& window) {
    //window.draw(boundbox);
    //root.draw(window);


    for(auto & body: bodies) {
        body.draw(window);
    }
}

void Simulator::updateBodies(sf::Int64 delta) {
    std::lock_guard<std::mutex> guard(mut);
    //double minforce = 0;
    //for(auto & i : bodies)
        //minforce = std::max(minforce, pow(i.acceleration.x,2) + pow(i.acceleration.y,2));
    //std::cout << minforce << std::endl;
    for(auto & body : bodies) body.update(delta, maxForce);
}

void Simulator::drawTree(sf::RenderWindow &window) {
    std::lock_guard<std::mutex> guard(mut);
    tree.draw(window);
}







