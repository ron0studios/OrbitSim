//
// Created by ron0 on 24/07/23.
//

#include <stack>
#include "QuadTree.h"

#include <iostream>

QuadTree::QuadTree() {
    r.setFillColor(sf::Color::Transparent);
    r.setOutlineThickness(2.0);
}

/**
 * Generates a QuadTree spatial data structure, given an array of bodies
 * @param bound the max distance from the origin of the tree (half the max width of the tree)
 * @param bodies the vector of bodies to generate the tree from
 */
QuadTree::QuadTree(double bound, std::vector<Body> *bodies, float brightness)
{
    this->brightness = brightness;
    r.setFillColor(sf::Color::Transparent);
    r.setOutlineThickness(2.0);
    this->bound = bound;

    tree.push_back({0,0.0,bound*2.0,0, 0.0, 0.0, nullptr}); // root

    // center x, center y
    double cx, cy;
    int i = 0;
    for(auto & body : *bodies){
        i++;
        std::stack<int> stack; stack.push(0);
        cx = cy = 0.0;

        if(abs(body.position.x) > bound)    continue;
        if(abs(body.position.y) > bound) continue;

        //std::cout << i << " " << body.position.x << " " << body.position.y << std::endl;


        while(!stack.empty()) {
            int idx = stack.top(); stack.pop();

            if(tree[idx].mass == 0) {
                tree[idx].mass = body.mass;
                tree[idx].singleChild = &body;
                tree[idx].total = 1;
                tree[idx].massx = body.position.x;
                tree[idx].massy = body.position.y;

                tree[idx].cx = cx;
                tree[idx].cy = cy;
                break;
            }

            tree[idx].mass += body.mass;
            double totalx = tree[idx].massx * tree[idx].total;
            double totaly = tree[idx].massy * tree[idx].total;
            totalx += body.position.x;
            totaly += body.position.y;

            tree[idx].total += 1;
            tree[idx].massx = totalx/tree[idx].total;
            tree[idx].massy = totaly/tree[idx].total;

            if(tree[idx].child > 0) {
                int quad = getQuadrant(cx,cy,body.position.x, body.position.y);

                if(quad%2)
                    cx += tree[idx].width/4.0;
                else
                    cx -= tree[idx].width/4.0;

                if(quad < 2)
                    cy -= tree[idx].width/4.0;
                else
                    cy += tree[idx].width/4.0;


                stack.push(tree[idx].child+quad);
            }
            else {
                tree[idx].child = (int)tree.size();
                for(int i = 0; i < 4; i++) {
                    double cx2 = cx;
                    double cy2 = cy;
                    if(i%2)
                        cx2 += tree[idx].width/4.0;
                    else
                        cx2 -= tree[idx].width/4.0;

                    if(i < 2)
                        cy2 -= tree[idx].width/4.0;
                    else
                        cy2 += tree[idx].width/4.0;

                    tree.push_back({0, 0.0, tree[idx].width / 2.0, 0, 0.0, 0.0, nullptr, cx2, cy2});
                }



                if(std::abs(tree[idx].singleChild->position.x-body.position.x) < 0.00000001
                or std::abs(tree[idx].singleChild->position.y-body.position.y) < 0.00000001){
                    throw std::logic_error("Two bodies cannot have the exact same position!");
                }

                int quadA = getQuadrant(sf::Vector2<double>(cx,cy), tree[idx].singleChild->position);

                tree[tree[idx].child+quadA].mass = tree[idx].singleChild->mass;
                tree[tree[idx].child+quadA].singleChild = tree[idx].singleChild;
                tree[tree[idx].child+quadA].total = 1;
                tree[tree[idx].child+quadA].massx = tree[idx].singleChild->position.x;
                tree[tree[idx].child+quadA].massy = tree[idx].singleChild->position.y;
                tree[idx].singleChild = nullptr;



                int quadB = getQuadrant(sf::Vector2<double>(cx,cy), body.position);

                if(quadB%2)
                    cx += tree[idx].width/4.0;
                else
                    cx -= tree[idx].width/4.0;

                if(quadB < 2)
                    cy -= tree[idx].width/4.0;
                else
                    cy += tree[idx].width/4.0;
                stack.push(tree[idx].child+quadB);
            }

        }
    }

    //std::cout << tree.size() << std::endl;
}

int QuadTree::getQuadrant(sf::Vector2<double> center, sf::Vector2<double> s) {
    if(s.x <= center.x){
        if(s.y <= center.y){
            return 0;
        }
        else
        {
            return 2;
        }
    }
    else {
        if(s.y <= center.y){
            return 1;
        }
        else
        {
            return 3;
        }
    }
}

int QuadTree::getQuadrant(double cx, double cy, double sx, double sy) {
    if(sx <= cx){
        if(sy <= cy){
            return 0;
        }
        else
        {
            return 2;
        }
    }
    else {
        if(sy <= cy){
            return 1;
        }
        else
        {
            return 3;
        }
    }
}

void QuadTree::updateForce(Body *body, double theta) {

    std::stack<int> stack; stack.push(0);
    body->acceleration = {0.0,0.0};

    while(!stack.empty()) {
        int idx = stack.top(); stack.pop();
        node n = tree[idx];

        if(n.mass == 0) continue;

        if(n.child > 0){
            double distance = sqrt(pow(n.massx-body->position.x,2) + pow(n.massy-body->position.y, 2));
            double ratio = n.width/(distance);

            if(ratio <= theta){

                sf::Vector2<double> accel = forcePair(body->mass,
                                                      n.mass,
                                                      body->position,
                                                      sf::Vector2<double>(n.massx,n.massy)) / body->mass;

                body->acceleration += accel;
            }
            else
            {
                for(int i = 0; i < 4; i++){
                    stack.push(n.child + i);
                }
            }

        }
        else
        {
            if(n.singleChild == body) continue;

            sf::Vector2<double> accel = forcePair(body->mass,
                                                  n.singleChild->mass,
                                                  body->position,
                                                  n.singleChild->position) / body->mass;

            body->acceleration += accel;
        }
    }

}



/**
 * calculates the force *on* mass A *from* mass B
 * @param massA
 * @param massB
 * @param posA
 * @param posB
 * @return force (divide by massA to get acceleration)
 */
sf::Vector2<double> QuadTree::forcePair(double massA, double massB, sf::Vector2<double> posA, sf::Vector2<double> posB)
{

    if(posA == posB){
        throw std::logic_error("Two bodies cannot have the exact same position!");
        return {0.0,0.0};
    }

    double softening = 10;
    double distance  = sqrt(pow(posB.x-posA.x,2) + pow(posB.y-posA.y, 2))/10;

    double mag = std::min((massA*massB)/(pow(distance,2) + pow(softening,2)), 300000000.0);



    /*
    if(mag >= 30000000){
        return {0.0,0.0};
    }
     */

    sf::Vector2<double> force = (posB-posA) * (mag/distance);

    return force;
}

void QuadTree::draw(sf::RenderWindow &window) {
    for(size_t i = 0; i < tree.size(); i++){
        sf::RectangleShape r(sf::Vector2f(tree[i].width, tree[i].width));
        r.setOrigin(tree[i].width/2, tree[i].width/2);
        /*sf::Vector2f position(0,0);
        int node = 0;
        int width = bound*2;
        while(node != i){
            std::cout << node << " " << i << std::endl;
            int quad = getQuadrant(position.x, position.y, tree[i].massx, tree[i].massy);
            if(i != 0 and tree[i].massx == 0 and tree[i].massy == 0)
                quad = getQuadrant(position.x, position.y, tree[i].singleChild->position.x, tree[i].singleChild->position.y);
            node = tree[i].child+quad;
            switch (quad){
                case 0:
                    position.x -= width/4;
                    position.y -= width/4;
                    break;
                case 1:
                    position.x += width/4;
                    position.y -= width/4;
                    break;
                case 2:
                    position.x -= width/4;
                    position.y += width/4;
                    break;
                case 3:
                    position.x += width/4;
                    position.y += width/4;
                    break;
            }
            width /= 2;
        }
        r.setPosition(position);*/

        r.setOutlineThickness(window.getView().getSize().x/1920);
        r.setFillColor(sf::Color::Transparent);
        r.setOutlineColor(sf::Color(255,255,255,(sf::Uint8)(255*brightness) ));
        r.setPosition(tree[i].cx, tree[i].cy);

        window.draw(r);
    }
}




