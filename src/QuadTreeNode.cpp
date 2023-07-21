//
// Created by ron0 on 20/07/23.
//

#include "QuadTreeNode.h"
#include <stack>

QuadTreeNode::QuadTreeNode(sf::Vector2<double> position, double mass, double width) {
    this->position = position;
    this->mass = mass;
    this->width = width;
    debug = sf::RectangleShape(sf::Vector2f(width,width));
    debug.setFillColor(sf::Color::Transparent);
    debug.setOutlineThickness(2.0);
    debug.setOrigin(sf::Vector2f(width/2.0, width/2.0));
    debug.setPosition((float)position.x, (float)position.y);
}

QuadTreeNode *QuadTreeNode::getChildFromIdx(int idx) {
    switch (idx) {
        case 0:
            return NW;
        case 1:
            return NE;
        case 2:
            return SW;
        case 3:
            return SE;
        default:
            return nullptr;
    }
}

QuadTreeNode *QuadTreeNode::getQuadrant(sf::Vector2<double> pos) {
    return getChildFromIdx(getQuadrantIdx(pos));
}

sf::Vector2<double> QuadTreeNode::getQuadrantCenter(QuadTreeNode *q){
    if(q==NW)
        return sf::Vector2<double>( position.x - width/4.0, position.y - width/4.0);
    if(q==NE)
        return sf::Vector2<double>( position.x + width/4.0, position.y - width/4.0);
    if(q==SW)
        return sf::Vector2<double>( position.x - width/4.0, position.y + width/4.0);
    if(q==SE)
        return sf::Vector2<double>( position.x + width/4.0, position.y + width/4.0);
}

void QuadTreeNode::draw(sf::RenderWindow &window) {
    std::stack<QuadTreeNode*> trace;
    trace.push(this);
    while(!trace.empty()){
        QuadTreeNode* top = trace.top(); trace.pop();
        window.draw(top->debug);

        if(top->debugLast) continue;

        for(int i = 0; i < 4; i++){
            QuadTreeNode* node = top->getChildFromIdx(i);
            if(node == nullptr) continue;
            trace.push(node);
        }
    }
}

int QuadTreeNode::getQuadrantIdx(sf::Vector2<double> pos){
    if(pos.x <= position.x){
        if(pos.y <= position.y){
            return 0;
        }
        else{
            return 2;
        }
    }
    else{
        if(pos.y <= position.y){
            return 1;
        }
        else{
            return 3;
        }
    }
}

void QuadTreeNode::setQuadrant(int idx, QuadTreeNode* val) {
    switch (idx) {
        case 0:
            NW = val;
            break;
        case 1:
            NE = val;
            break;
        case 2:
            SW = val;
            break;
        case 3:
            SE = val;
            break;
        default:
            break;
    }
}

bool QuadTreeNode::checkPoint(sf::Vector2<double> pos) {
    if( pos.x <= position.x+width/2.0 and pos.x >= position.x-width/2.0
    and pos.y <= position.y+width/2.0 and pos.y >= position.y-width/2.0)
        return true;
    return false;
}

sf::Vector2<double> QuadTreeNode::getQuadrantCenter(int q) {
    if(q==0)
        return sf::Vector2<double>( position.x - width/4.0, position.y - width/4.0);
    if(q==1)
        return sf::Vector2<double>( position.x + width/4.0, position.y - width/4.0);
    if(q==2)
        return sf::Vector2<double>( position.x - width/4.0, position.y + width/4.0);
    if(q==3)
        return sf::Vector2<double>( position.x + width/4.0, position.y + width/4.0);
}

