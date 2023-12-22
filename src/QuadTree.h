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


        struct node {
            int child   = 0;
            double mass = 0.0;
            double width = 0.0;


            // center of mass calculations
            int total   = 0;
            double massx = 0.0;
            double massy = 0.0;

            // if node has 1 child
            Body* singleChild = nullptr;

            double cx = 0.0;
            double cy = 0.0;
        };

       std::vector<node> tree;
};


#endif //ORBITSIM_QUADTREE_H
