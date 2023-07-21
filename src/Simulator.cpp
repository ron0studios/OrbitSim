//
// Created by ron0 on 19/07/23.
//

#include "Simulator.h"
#include <iostream>
#include <stack>
#include <thread>
#include "ctpl.h"


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
        //std::cout << id << std::endl;

        //printf("%i\n", id);
        sf::Vector2<double> bpos = body.position;

        if(abs(bpos.x) > bounds or abs(bpos.y) > bounds) {
            continue;
        }

        QuadTreeNode* top = &root;

        while(true) {
            // deduce quadrant
            int part = top->getQuadrantIdx(bpos);
            //printf("part: %i\n", part);


            if(top->getChildFromIdx(part) != nullptr)
            {
                top->getChildFromIdx(part)->mass += body.mass;
                top = top->getChildFromIdx(part);
            }
            else
            {
                if(top->singleNodeQuadrant != -1)
                {
                    while(true) {
                        sf::Vector2<double> posA = top->getQuadrantCenter(top->singleNodeQuadrant);
                        sf::Vector2<double> posB = top->getQuadrantCenter(part);
                        if(part == top->singleNodeQuadrant){
                            top->setQuadrant(part, new QuadTreeNode(posA, top->mass, top->width/2));

                            top->singleNodeQuadrant = -1;
                            sf::Vector2<double> tmp = top->singleNodePos;
                            double tmpmass = top->singleNodeMass;
                            Body* tmpref   = top->singleNodeRef;
                            top = top->getChildFromIdx(part);
                            top->singleNodeQuadrant = top->getQuadrantIdx(tmp);
                            top->singleNodePos = tmp;
                            top->singleNodeMass = tmpmass;
                            top->singleNodeRef = tmpref;

                            part = top->getQuadrantIdx(bpos);
                            continue;
                        }

                        top->setQuadrant(top->singleNodeQuadrant, new QuadTreeNode(posA, top->mass, top->width / 2));
                        top->setQuadrant(part, new QuadTreeNode(posB, body.mass, top->width / 2));

                        top->getChildFromIdx(part)->singleNodeQuadrant = top->getChildFromIdx(part)->getQuadrantIdx(
                                bpos);
                        top->getChildFromIdx(top->singleNodeQuadrant)->singleNodeQuadrant = top->getChildFromIdx(
                                top->singleNodeQuadrant)->getQuadrantIdx(top->singleNodePos);

                        top->getChildFromIdx(part)->singleNodeMass = body.mass;
                        top->getChildFromIdx(top->singleNodeQuadrant)->singleNodeMass = top->singleNodeMass;
                        top->getChildFromIdx(part)->singleNodePos = bpos;
                        top->getChildFromIdx(top->singleNodeQuadrant)->singleNodePos = top->singleNodePos;
                        top->getChildFromIdx(part)->singleNodeRef = &body;
                        top->getChildFromIdx(top->singleNodeQuadrant)->singleNodeRef = top->singleNodeRef;

                        top->singleNodeQuadrant = -1;
                        break;
                    }
                    break;
                }

                sf::Vector2<double> pos = top->getQuadrantCenter(part);
                top->setQuadrant(part, new QuadTreeNode(pos, body.mass, top->width/2));
                top->getChildFromIdx(part)->singleNodeQuadrant = top->getChildFromIdx(part)->getQuadrantIdx(bpos);
                top->getChildFromIdx(part)->singleNodePos = bpos;
                top->getChildFromIdx(part)->singleNodeMass = body.mass;
                top->getChildFromIdx(part)->singleNodeRef = &body;

                top->singleNodeQuadrant = -1;
                break;
            }
        }
    }
}



// postorder traversal
void Simulator::delQuadTree(QuadTreeNode* node) {
    if(node == nullptr)
        return;

    for(int i = 0; i < 4; i++){
        if(node->getChildFromIdx(i) == nullptr)
            continue;

        delQuadTree(node->getChildFromIdx(i));
    }

    if(node == &root) {
        root = QuadTreeNode(sf::Vector2<double>(0.0, 0.0), 0.0, bounds * 2.0);
        return;
    }
    delete node;
    node = nullptr;
}



void Simulator::updateTree() {
    delQuadTree(&root);
    genQuadTree();
}


void Simulator::calcForce(Body &body) {

    // traverse tree
    body.acceleration = sf::Vector2<double>(0.0, 0.0);

    std::stack<QuadTreeNode*> trace;
    trace.push(&root);
    while(!trace.empty()){
        QuadTreeNode* node = trace.top(); trace.pop();

        if(node->singleNodeQuadrant != -1)
        {
            if(node->singleNodeRef == &body)
                continue;


            double magnitude = (body.mass * node->singleNodeMass)/( 0.1 * squarelen(node->singleNodePos-body.position));
            //if(isnan(magnitude) or isinf(magnitude)) magnitude = 3 * pow(10,7); // 10% speed of light


            sf::Vector2<double> force = node->singleNodePos-body.position;
            //std::cout << "\t" << force.x << " " << force.y << std::endl;
            force.x *= magnitude/ sqrt(squarelen(node->singleNodePos-body.position));
            force.y *= magnitude/ sqrt(squarelen(node->singleNodePos-body.position));


            sf::Vector2<double> accel(force.x/body.mass, force.y/body.mass);
            body.acceleration += accel;
            continue;
        }

        double ratio = node->width/ sqrt(squarelen(node->position- body.position));
        if(ratio < 0.5) {
            double magnitude = (body.mass * node->mass)/( 0.1 * squarelen(node->position-body.position));
            //if(isnan(magnitude) or isinf(magnitude)) magnitude = 3 * pow(10,7); // 10% speed of light


            sf::Vector2<double> force = node->position-body.position;
            //std::cout << "\t" << force.x << " " << force.y << std::endl;
            force.x *= magnitude/ sqrt(squarelen(node->position-body.position));
            force.y *= magnitude/ sqrt(squarelen(node->position-body.position));


            sf::Vector2<double> accel(force.x/body.mass, force.y/body.mass);
            body.acceleration += accel;

        }
        else {
            for (int i = 0; i < 4; i++) {
                if (node->getChildFromIdx(i) == nullptr)
                    continue;

                trace.push(node->getChildFromIdx(i));
            }
        }
    }
}

void Simulator::updateForces() {
    //std::cout << "loop\n" << std::endl;




    int num_threads = 16;
    std::vector<std::thread> threads(num_threads);

    for(int i = 0; i < num_threads; i++){
        threads[i] = std::thread([this](int i, int n){
                for(int j = i*n; j < ((i+1)*n); j++)
                    calcForce(bodies[j]);
            },i, bodies.size()/num_threads);
    }
    /*
    for(int i=0; i < bodies.size(); i++) {


        //threads[i] = std::thread([this](Body& b){calcForce(b);}, std::ref(bodies[i]));
    }
     */
    for(int i = 0; i < num_threads; i++)
        threads[i].join();

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
    //root.draw(window);

    for(auto & body: bodies) {
        body.draw(window);
    }
}

void Simulator::updateBodies(sf::Int64 delta) {
    for(auto & body : bodies) body.update(delta);
}






