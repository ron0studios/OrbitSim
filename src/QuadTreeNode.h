//
// Created by ron0 on 20/07/23.
//

#include <SFML/Graphics.hpp>

#ifndef ORBITSIM_QUADTREENODE_H
#define ORBITSIM_QUADTREENODE_H



class QuadTreeNode {
    public:
        double mass = 0.0;
        double width = 0.0;
        sf::RectangleShape debug;
        sf::Vector2<double>  position;

        int singleNodeQuadrant = -1;
        sf::Vector2<double>  singleNodePos;
        double singleNodeMass;

        QuadTreeNode* NW = nullptr;
        QuadTreeNode* NE = nullptr;
        QuadTreeNode* SW = nullptr;
        QuadTreeNode* SE = nullptr;

        QuadTreeNode(sf::Vector2<double> position, double mass, double width);

        // in case you don't want to do NW,Ne ... etc
        QuadTreeNode* getChildFromIdx(int idx);
        int getQuadrantIdx(sf::Vector2<double> pos);
        QuadTreeNode* getQuadrant(sf::Vector2<double> pos);
        sf::Vector2<double> getQuadrantCenter(QuadTreeNode* q);
        sf::Vector2<double> getQuadrantCenter(int q);
        void setQuadrant(int idx, QuadTreeNode* val);
        bool checkPoint(sf::Vector2<double> pos);

        void draw(sf::RenderWindow& window);

};


#endif //ORBITSIM_QUADTREENODE_H
