//
// Created by ron0 on 19/07/23.
//
#include "Body.h"
#include "QuadTreeNode.h"
#include <vector>
#include <math.h>

#ifndef ORBITSIM_SIMULATOR_H
#define ORBITSIM_SIMULATOR_H


class Simulator {
    public:
        std::vector<Body> bodies;
        Simulator(double bounds = 50000);
        void addBody(Body body);
        void updateForces();
        void updateTree();
        void updateBodies(sf::Int64 delta);
        void draw(sf::RenderWindow& window);

    private:
        double squarelen(sf::Vector2<double> a);
        void genQuadTree();
        void delQuadTree(QuadTreeNode* node);
        void calcForce(Body& body);

        QuadTreeNode root = QuadTreeNode(sf::Vector2<double>(0.0,0.0), 0.0, 0.0);;
        double bounds;
        sf::RectangleShape boundbox;

};


#endif //ORBITSIM_SIMULATOR_H
