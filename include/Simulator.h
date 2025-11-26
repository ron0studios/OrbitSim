//
// Created by ron0 on 19/07/23.
//
#include "Body.h"
#include "QuadTree.h"
#include <vector>
#include <math.h>
#include <mutex>

#ifndef ORBITSIM_SIMULATOR_H
#define ORBITSIM_SIMULATOR_H


class Simulator {
    public:
        std::vector<Body> bodies;
        Simulator(double bounds = 50000);
        void addBody(Body body);
        void updateForces(bool bruteForce=false);
        void updateTree(float brightness);
        void updateBodies(sf::Int64 delta);
        void draw(sf::RenderWindow& window);
        void drawTree(sf::RenderWindow& window);

    private:
        std::mutex mut;
        double maxForce = -1;

        double bounds;
        sf::RectangleShape boundbox;
        QuadTree tree;

};


#endif //ORBITSIM_SIMULATOR_H
