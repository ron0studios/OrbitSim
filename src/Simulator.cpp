//
// Created by ron0 on 19/07/23.
//

#include "Simulator.h"
#include <iostream>
#include <stack>
#include <thread>
#include <cfloat>
#include "ctpl.h"


// initialises the simulation by creating a bounding box rectangle
Simulator::Simulator(double bounds) {
    this->bounds = bounds;

    boundbox = sf::RectangleShape(sf::Vector2f(bounds*2,bounds*2));
    boundbox.setPosition(sf::Vector2f(-bounds, -bounds));
    boundbox.setFillColor(sf::Color::Transparent);
    boundbox.setOutlineThickness(100);
}

// appends a body object to the bodies list
void Simulator::addBody(Body body) {
    bodies.push_back(body);
}

// destroys the current tree object and creates a new one
// a mutex object is present to prevent race conditions with any of the
// quad tree nodes which may end up getting shared
void Simulator::updateTree(float brightness) {
    std::lock_guard<std::mutex> guard(mut);

    tree = QuadTree(this->bounds, &bodies, brightness);
}

// updates all the forces for every body in the simulation
// a mutex lock is present because there is some multithreading
// operating on the bodies list which may lead to a race condition
// crashing the program 
void Simulator::updateForces(bool bruteForce) {
    std::lock_guard<std::mutex> guard(mut);
    maxForce = -1;

    // if the bruteforce parameter is set we still use multithreading.
    // However, we update the forces without the barnes hut algorithm 
    // but rather by looping through every body for each body, in an 
    // O(N^2) algorithm. It is more accurate but painfully slow
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
        return;
    }


    // if the bruteforce parameter is not set we use the quadtree to update
    // each of the bodies using tree.updateForce
    int num_threads = 16;
    std::vector<std::thread> threads(num_threads);

    for(int i = 0; i < num_threads; i++){
        threads[i] = std::thread([this](int i, int n){

            for(int j = i*n; j < ((i+1)*n) and j < bodies.size(); j++) {
                tree.updateForce(&bodies[j], 0.5);
                maxForce = std::max(maxForce, std::sqrt(std::pow(bodies[j].velocity.x,2) + std::pow(bodies[j].velocity.y,2)));
            }

                //calcForce(bodies[j]);
        },i, std::ceil((double)bodies.size()/num_threads));
    }


    for(int i = 0; i < num_threads; i++)
        threads[i].join();
}

// draws each body by iterating through the bodies list
void Simulator::draw(sf::RenderWindow& window) {
    for(auto & body: bodies) {
        body.draw(window);
    }
}

// updates each body by iterating through the bodies list
void Simulator::updateBodies(sf::Int64 delta) {
    std::lock_guard<std::mutex> guard(mut);
    for(auto & body : bodies) body.update(delta, maxForce);
}

// draws the quadtree by accessing the private tree variable
void Simulator::drawTree(sf::RenderWindow &window) {
    std::lock_guard<std::mutex> guard(mut);
    tree.draw(window);
}







