//
// Created by ron0 on 19/07/23.
//

#include "Body.h"
#include <algorithm>
#include <math.h>

// initialises the body object with a mass, radius, position, velocity and radius for the
// circleobject to be rendered
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


// a helper function to convert a range of values to a colour between
// blue and red in a hsv fashion, but returned as an sf hsv color
sf::Color Body::convert_to_rgb(double min, double max, double val){
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

// updates the properties of the object based on the time passed by delta
void Body::update(sf::Int64 delta, double maxForce) {
    int totForce = sqrt(pow(velocity.x,2) + pow(velocity.y,2));// * mass;
    shape.setFillColor(convert_to_rgb(0, maxForce, totForce));
    velocity += acceleration * (delta/ 1000000.0);
    position += velocity * (delta/ 1000000.0);

}

// draws the body in non simple mode by creating a circleshape and
// using the window parameter to draw the body. in debug mode there are also
// arrows drawn for the acceleration and velocity of the body
void Body::draw(sf::RenderWindow &window) {
    shape.setPosition((float)position.x, (float)position.y);
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    window.draw(shape);
#if DEBUG
    if(debug) {
        veldir.setPosition((float) position.x, (float) position.y);
        double vrot = atan2(velocity.y, velocity.x);
        veldir.setRotation(vrot * (180 / M_PI));

        accdir.setPosition((float) position.x, (float) position.y);
        double arot = atan2(acceleration.y, acceleration.x);
        accdir.setRotation(arot * (180 / M_PI));
    }

    if(debug) {
        window.draw(veldir);
        window.draw(accdir);
    }
#endif
}

