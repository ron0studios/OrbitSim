//
// Created by ron0 on 24/07/23.
//

#ifndef ORBITSIM_QUADTREE_H
#define ORBITSIM_QUADTREE_H

#include <SFML/Graphics.hpp>
#include "Body.h"
#include <cmath>
#include <mutex>
#include "globals.h"

class QuadTree {
    public:
        QuadTree(double bound, std::vector<Body>* bodies, float brightness);
        QuadTree();

        void updateForce(Body* body, double theta = 0.5);
        void draw(sf::RenderWindow& window);
        static sf::Vector2<double> forcePair(double massA, double massB, sf::Vector2<double> posA, sf::Vector2<double> posB);
    private:

        sf::RectangleShape r;
        static int getQuadrant(sf::Vector2<double> center, sf::Vector2<double> s);
        static int getQuadrant(double cx, double cy, double sx, double sy);
        double bound;
        float brightness = 0.04;

        // the node struct is declared as an attribute of the quadtree since a quadtree consists
        // of several nodes, making accessing node properties more convenient. 
        struct node {
            int child   = 0; // the index of the first child of the node. children are stored contiguously
            double mass = 0.0; // the total mass of the node and its children
            double width = 0.0; // the width of the node


            // center of mass calculations
            int total   = 0; // total bodies of the node
            double massx = 0.0; // x position of center of mass
            double massy = 0.0; // y position of center of mass

            // if node has 1 child
            Body* singleChild = nullptr; // pointer to singleChild body

            double cx = 0.0; // x position of the center of the node
            double cy = 0.0; // y position of the center of the node
        };

        // a list of nodes representing the quadtree
       std::vector<node> tree;
};


#endif //ORBITSIM_QUADTREE_H
