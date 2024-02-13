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

    // creates the boundary rectangle of the quad tree
    r.setFillColor(sf::Color::Transparent);
    r.setOutlineThickness(2.0);
    this->bound = bound;

    // push back the root of the tree
    tree.push_back({0,0.0,bound*2.0,0, 0.0, 0.0, nullptr}); // root

    // center x, center y
    double cx, cy;
    int i = 0; // iterator counter
    for(auto & body : *bodies){ 
        i++;

        // for each body we create a stack with a single element
        // we use this to begin traversing the quadtree to create or amend nodes
        std::stack<int> stack; stack.push(0);
        cx = cy = 0.0;

        // we stop proceeding further if the node is outside the quadtree's bounds
        if(abs(body.position.x) > bound)    continue;
        if(abs(body.position.y) > bound) continue;


        // while there are still elements in the stack we pop the topmost element and 
        // process that stack item
        while(!stack.empty()) {
            int idx = stack.top(); stack.pop();

            // if the current tree node has no mass, it must be both empty and have no
            // singleChild, so we instantiate the relevant values and escape
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

            // if the current tree node has some mass, we may have to traverse deeper into the
            // quadtree so we update the values of the current node such as the total mass, or 
            // total body count
            tree[idx].mass += body.mass;
            double totalx = tree[idx].massx * tree[idx].total;
            double totaly = tree[idx].massy * tree[idx].total;
            totalx += body.position.x;
            totaly += body.position.y;

            tree[idx].total += 1;
            tree[idx].massx = totalx/tree[idx].total;
            tree[idx].massy = totaly/tree[idx].total;

            // if the current node has children we find out which child the body should be in
            // based on quadrant and traverse to that node by placing it to the top of the stack
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

                // if the current node has no children we create 4 empty children
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


                // if the current node's singleChild coincides with the body we're trying to add
                // we throw an error
                if(std::abs(tree[idx].singleChild->position.x-body.position.x) < 0.00000001
                or std::abs(tree[idx].singleChild->position.y-body.position.y) < 0.00000001){
                    throw std::logic_error("Two bodies cannot have the exact same position!");
                }

                // we calculate the quadrant that the singleChild should be in and update the relevant
                // child node with a new singleChild
                int quadA = getQuadrant(sf::Vector2<double>(cx,cy), tree[idx].singleChild->position);

                tree[tree[idx].child+quadA].mass = tree[idx].singleChild->mass;
                tree[tree[idx].child+quadA].singleChild = tree[idx].singleChild;
                tree[tree[idx].child+quadA].total = 1;
                tree[tree[idx].child+quadA].massx = tree[idx].singleChild->position.x;
                tree[tree[idx].child+quadA].massy = tree[idx].singleChild->position.y;
                tree[idx].singleChild = nullptr;



                // we find out the quadrant of the body we're trying to add and traverse to that node.
                // It may be that this quadrant is the same one as the singleChild, in which we may have to 
                // repeat this process several times to differentiate the two into their own respective
                // quadrants
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

}

// calcluates the quadrant 0,1,2,4 => NW,NE,SW,SE of a node relative to a provided center
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

// recursively updates the force of a given body 
// using the quadtree and an approximation factor, theta
void QuadTree::updateForce(Body *body, double theta) {

    // since we need to traverse the quadtree we create a stack to backtrack and
    // move deeper into the tree
    std::stack<int> stack; stack.push(0);
    body->acceleration = {0.0,0.0}; // reset the previous acceleration of the body

    // while the stack still contains items we pop
    // from the top of the stack and process it
    while(!stack.empty()) {
        int idx = stack.top(); stack.pop();
        node n = tree[idx];

        if(n.mass == 0) continue; // if the node is empty we will have reached an empty node and quit

        // if the current node has children we calculate the ratio between the node's width and distance
        // from the body to the node's center of mass. If this ratio is less than theta, we can approximate
        // the whole node as a single heavy body and cull the tree.
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
            // if the node has no children then it must have a singleBody.
            // We simply calculate the force between body and singleBody.
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


    sf::Vector2<double> force = (posB-posA) * (mag/distance);

    return force;
}

// draws the quadtree by iterating through each node and drawing a rectangle shape
void QuadTree::draw(sf::RenderWindow &window) {
    for(size_t i = 0; i < tree.size(); i++){
        sf::RectangleShape r(sf::Vector2f(tree[i].width, tree[i].width));
        r.setOrigin(tree[i].width/2, tree[i].width/2);
        r.setOutlineThickness(window.getView().getSize().x/1920);
        r.setFillColor(sf::Color::Transparent);
        r.setOutlineColor(sf::Color(255,255,255,(sf::Uint8)(255*brightness) ));
        r.setPosition(tree[i].cx, tree[i].cy);

        window.draw(r);
    }
}




