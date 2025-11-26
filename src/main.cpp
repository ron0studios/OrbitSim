#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include "Simulator.h"
#include "GUI.h"
#include <thread>
#include <chrono>

// a function to create a custom galaxy or cluster shape based on certain arguments
// sim: the simulator object in which to instantiate the bodies
// starnum: the total number of stars to create
// starmass: the mass of each star on average
// c_mass: the mass of a central star, e.g. the sun or a black hole. If set to 0, this object wont be created
// rad: the radius of the galaxy or cluster
// rotspeed: the tangential velocity of each planet
// cx: the x position of the center of the cluster
// cy: the y position of the center of the cluster
// c_velx: the velocity of the cluster in the x direction relative to other galaxies
// c_vely: the velocity of the cluster in the y direction relative to other galaxies
// radial: the outward radial velocity of the stars
// massvar: the range of values that the starmass could take.
void addGalaxy(Simulator& sim, double starnum, double starmass, double c_mass, double rad, double rotspeed, double cx, double cy, double c_velx, double c_vely, double radial=0, double massvar = 10.0, double gap = 0.0){
    int num_threads = 40;
    std::vector<std::thread> threads(num_threads);
    std::vector<Body*> ptrarr((int)starnum,nullptr);

    // 16 threads are created to iterate through 1/16th of the starnum
    for(int i = 0; i < num_threads; i++){
        threads[i] = std::thread([&](int i, int n){
            for(int j = i*n; j < ((i+1)*n); j++) {
                if(j >= starnum) break;
                // the following maths uses polar coordinates to position each of the stars randomly in space
                double radius = rad;
                double rnd = rand();
                double r = radius * sqrt( pow((double)rand()/RAND_MAX, 2)) + gap;
                double theta = ((double)rnd/RAND_MAX) * 2 * M_PI;
                double x = r * cos(theta);
                double y = r * sin(theta);

                // the following maths calculates the velocity of each star based on its angle to the center
                double rnd2 = (((double)rand()/ RAND_MAX)*massvar);
                double velx = -cos(M_PI*0.5 - ( ((double)rnd/ RAND_MAX) * 2*M_PI )) *  200 * (2- r/radius) * rotspeed;
                double vely =  sin(M_PI*0.5 - ( ((double)rnd/ RAND_MAX) * 2*M_PI )) *  200 * (2- r/radius) * rotspeed;

                // the following applies the radial velocity to each star
                double velradx = radial*x;
                double velrady = radial*y;
                ptrarr[j] = new Body(starmass * rnd2 * 10,20 * rnd2,sf::Vector2<double>(x+cx, y+cy), sf::Vector2<double>(velx + c_velx + velradx,vely + c_vely + velrady));
            }

        },i,std::ceil((double)starnum/num_threads));
    }

    // the 16 threads are before exiting scope to prevent memory issues
    for(int i = 0; i < num_threads; i++) {
        threads[i].join();
    }

    // nullcount is recorded as a precautionary measure of how many threads failed to execute
    int nullcount = 0;
    for(int i = 0; i < starnum; i++){
        if(ptrarr[i] == nullptr){
            nullcount++;
        }
        sim.addBody(*ptrarr[i]);
    }
    std::cout << nullcount << std::endl;

    // since all bodies are now added to the simulation, the original pointers can be deleted to avoid memory leaks
    for(Body* b : ptrarr)
        delete b;
    ptrarr.clear();

    if(c_mass) sim.addBody(Body(c_mass,10,sf::Vector2<double>(cx, cy), sf::Vector2<double>(c_velx, c_vely)));
}

int main() {
    double speed = 0.5;    // the speed multiplier of moving around the simulation
    double window_width  = 1920;
    double window_height = 1080;

    // the render window is instantiated
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "OrbitSim");
    window.setPosition(sf::Vector2i(0,0));
    window.setFramerateLimit(60);
    int sizex = 1920*1;
    int sizey = 1080*1;

    // Create GUI
    GUI gui;
    gui.init(window);

    // a view is instantiated to navigate the simulation space
    sf::View view(sf::FloatRect(-sizex/2.0f, -sizey/2.0f, sizex, sizey));
    window.setView(view);

    // the simulator is instantiated with a large bounding box, but not too large
    // to avoid creating a recursion depth error
    Simulator space(10000000);
    float timescale  = 1; // a multiplier to the deltatime to alter the simulation speed
    bool paused = false; // to pause or unpause the simulation
    bool render_tree = true; // to activate the rendering of the quad tree
    bool use_colors = true; // whether to use color highlighting or black and white
    bool simple_render = false; // whether to render with pixels or polygons
    bool render_arrows = false; // whether to render arrows for velocity and acceleration on each object
    int scale = 4; // the scale of pixels in simple rendering mode
    float brightness = 1; // the brightness of the simulation colours (includes blending)
    float tree_brightness = 0.04; // the brightness of the quadtree (includes blending)
    
    sf::Clock deltaClock;
    sf::Time dt = deltaClock.restart();
    int iterations = 0;

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            gui.handleEvent(event, window);
            
            if (event.type == sf::Event::Closed)
                window.close();
                
            // Handle non-GUI events here (camera movement, etc.)
        }

        gui.update(window, dt);
        
        // Check GUI state
        if (gui.shouldResetSimulation()) {
            space.bodies.clear();
            // Add initial galaxy or setup
            addGalaxy(space, 10000, 0.00001, 100000, 100, 0.001, 0, 0, 0, 0, 0, 0.5, 0);
        }
        
        paused = gui.isPaused();
        render_tree = gui.shouldShowTree();
        timescale = gui.getTimeStep();
        
        // Update simulation
        if (!paused) {
            space.updateForces();
            space.updateBodies(dt.asMilliseconds() * timescale);
        }

        // Render simulation
        window.clear();
        
        // Update tree for rendering
        space.updateTree(tree_brightness);
        
        // Render bodies
        space.draw(window);
        
        // Render QuadTree if enabled
        if (render_tree && !space.bodies.empty()) {
            space.drawTree(window);
        }
        
        // Render GUI
        gui.render();
        window.display();

        dt = deltaClock.restart();
        if(paused) iterations++;
    }

    gui.shutdown();
    return 0;
}