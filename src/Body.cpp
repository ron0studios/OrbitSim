//
// Created by ron0 on 19/07/23.
//

#include "Body.h"
#include <algorithm>
#include <math.h>

Body::Body(float mass, float radius, sf::Vector2<double> position, sf::Vector2<double> velocity) {
    this->mass = mass;
    this->radius = radius;
    this->position = position;


    this->velocity = velocity;
    acceleration = sf::Vector2<double>(0.0, 0.0);

    shape.setRadius(radius);
    shape.setOrigin(radius, radius);

#if DEBUG
    this->shape = sf::CircleShape(radius);
    this->shape.setPosition((float)position.x, (float)position.y);
    this->shape.setOrigin(radius,radius);

    // debug
    this->veldir = sf::RectangleShape(sf::Vector2f(50.0, 3.0));
    this->accdir = sf::RectangleShape(sf::Vector2f(50.0, 3.0));
    accdir.setFillColor(sf::Color(100,100,100));
#endif
}
/*def convert_to_rgb(minimum, maximum, value):
    minimum, maximum = float(minimum), float(maximum)
    halfmax = (minimum + maximum) / 2
    if minimum <= value <= halfmax:
        r = 0
        g = int( 255./(halfmax - minimum) * (value - minimum))
        b = int( 255. + -255./(halfmax - minimum)  * (value - minimum))
        return (r,g,b)
    elif halfmax < value <= maximum:
        r = int( 255./(maximum - halfmax) * (value - halfmax))
        g = int( 255. + -255./(maximum - halfmax)  * (value - halfmax))
        b = 0
        return (r,g,b)*/

sf::Color Body::convert_to_rgb(double min, double max, double val){

    //min = 0;
    //max = 1;
    //val = 1/(1+std::exp(-val));

    double halfmax = (min + max) / 2.0;
    int r,g,b;
    if(min <= val and val <= halfmax){
        r = 0;
        g = (int)(255.0/ (halfmax-min)*(val-min));
        b = (int)(255.0 + -255.0/(halfmax-min)*(val-min));
    }
    else{
        r = (int)(255.0/(max-halfmax)*(val-halfmax));
        g = (int)(255.0 + -255.0/(max-halfmax) * (val-halfmax));
        b = 0;
    }
    return sf::Color(r,g,b, 255.0);
}

void Body::update(sf::Int64 delta, double maxForce) {
    int totForce = sqrt(pow(velocity.x,2) + pow(velocity.y,2));// * mass;
    //shape.setFillColor(sf::Color(100+std::min(totForce,155), 100+std::min(155,totForce), 255-totForce));
    shape.setFillColor(convert_to_rgb(0, maxForce, totForce));
    velocity += acceleration * (delta/ 1000000.0);
    position += velocity * (delta/ 1000000.0);

}

void Body::draw(sf::RenderWindow &window) {
    shape.setPosition((float)position.x, (float)position.y);
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    //shape.setFillColor(sf::Color(  std::min(255, (int)sqrt(velocity.x*velocity.x + velocity.y*velocity.y)) ,   255- std::min(255, (int)sqrt(velocity.x*velocity.x + velocity.y*velocity.y))  ,0));

    //int totForce = std::min((sqrt(pow(acceleration.x,2) + pow(acceleration.y,2))/ 10) * 255, (double)255);
    //shape.setFillColor(sf::Color(totForce, totForce, 255-totForce));


    window.draw(shape);
#if DEBUG
    if(debug) {
        veldir.setPosition((float) position.x, (float) position.y);
        //veldir.setSize(sf::Vector2f(sqrt(velocity.x*velocity.x + velocity.y*velocity.y), 3.0));
        double vrot = atan2(velocity.y, velocity.x);
        veldir.setRotation(vrot * (180 / M_PI));

        accdir.setPosition((float) position.x, (float) position.y);
        //veldir.setSize(sf::Vector2f(sqrt(acceleration.x*acceleration.x + acceleration.y*acceleration.y), 3.0));
        double arot = atan2(acceleration.y, acceleration.x);
        accdir.setRotation(arot * (180 / M_PI));
    }

    if(debug) {
        window.draw(veldir);
        window.draw(accdir);
    }
#endif
}

