//
// Created by ron0 on 19/07/23.
//
#include "Body.h"
#include <vector>
#include <math.h>

#ifndef ORBITSIM_SIMULATOR_H
#define ORBITSIM_SIMULATOR_H


class Simulator {
    public:
        std::vector<Body> bodies;
        Simulator();
        void addBody(Body body);
        void update();
        void draw(sf::RenderWindow& window);
};


#endif //ORBITSIM_SIMULATOR_H
