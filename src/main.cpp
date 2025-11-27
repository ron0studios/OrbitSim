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



void addRing(Simulator& sim, double starnum, double starmass, double c_mass, double rad, double rotspeed, double cx, double cy, double c_velx, double c_vely, double radial=0, double massvar = 10.0, double gap = 0.0){
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
                //double velx = -cos(M_PI*0.5 - (theta)) *  500 * (2- r/radius) * rotspeed;
                //double vely =  sin(M_PI*0.5 - (theta)) *  500 * (2- r/radius) * rotspeed;

                double velx = -cos(M_PI*0.5 - theta)* sqrt((c_mass+starmass*10)/(0.001*gap));
                double vely = sin(M_PI*0.5 - theta)*  sqrt((c_mass+starmass*10)/(0.001*gap));

                // the following applies the radial velocity to each star
                double velradx = radial*x;
                double velrady = radial*y;
                ptrarr[j] = new Body(starmass * 10,20 * rnd2,sf::Vector2<double>(x+cx, y+cy), sf::Vector2<double>(velx + c_velx + velradx,vely + c_vely + velrady));
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


int main()
{
    double speed = 0.5;    // the speed multiplier of moving around the simulation
    double window_width  = 1920;
    double window_height = 1080;

    // the render window is instantiated
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "OrbitSim");
    window.setPosition(sf::Vector2i(0,0));
    
    // Create and initialize GUI
    GUI gui;
    gui.init(window);
    gui.setWindowSize(window_width, window_height);
    
    window.setFramerateLimit(60);
    int sizex = 1920*1;
    int sizey = 1080*1;

    // a view is instantiated to navigate the simulation space
    sf::View view(sf::FloatRect(-sizex/2.0f, -sizey/2.0f, sizex, sizey));
    window.setView(view);


    // the simulator is instantiated with a large bounding box, but not too large
    // to avoid creating a recursion depth error
    Simulator space(10000000);
    bool paused = false; // to pause or unpause the simulation
    sf::RectangleShape selectionBox; // the selection box object for drag selecting multiple bodies


    //addRing(space,10000,0.00001,100000,100,1,0,0,0,0,0,0.5,500);

    // a preliminary check to see whether any bodies are directly coinciding with each other. 
    // if this is the case we notify the user, since the simulation cannot proceed with direct collision
    for(int i = 0; i < space.bodies.size(); i++)
    {
        for(int j = i+1; j < space.bodies.size(); j++)  {
            if(space.bodies[i].position.x == space.bodies[j].position.x and space.bodies[i].position.y == space.bodies[j].position.y)
                throw std::domain_error("two bodies cannot have the same coordinate!");
        }

    }
    std::cout << "OBJECTS LOADED" << std::endl;



    sf::Clock deltaClock; // a timer to measure the length of each physics frame
    sf::Time dt; // the variable to store each frame's measured time in 
    sf::Uint8* pix; // a vector of pixels to render during simple mode
    int iterations = 0; // a count of how many iterations the simulation has gone through

    // begin the main game loop
    while (window.isOpen())
    {

        sf::Event event; // an sfml event to detect user input
        while (window.pollEvent(event)) // while the event queue is non empty each frame
        {
            gui.handleEvent(event, window);
            // if the X key on the window is pressed we close the window
            if (event.type == sf::Event::Closed)                       window.close(); 
            // if the space key is pressed we toggle the pause state of the simulation
            if(event.type == sf::Event::KeyReleased and event.key.code == sf::Keyboard::Space) {
                paused = !paused;
            }

            // handling left mouse button presses
            if(event.type == sf::Event::MouseButtonReleased and event.key.code == sf::Mouse::Left){
                // this gets the position of the mouse IN SIMULATION COORDINATES instead of window coordinates
                sf::Vector2<double> pos = (sf::Vector2<double>)window.mapPixelToCoords(sf::Mouse::getPosition());

                // if the mouse is not interacting with any ImGui objects we can proceed to simulation interaction
                if(!ImGui::IsAnyItemActive() and !ImGui::IsAnyItemFocused() and !ImGui::IsAnyItemHovered()) {
                    // checks each possible brush type and creates the associated cluster
                    std::string brush = gui.getSelectedBrush();
                    if (brush == "single")
                        addGalaxy(space, 0, 0, 1000, 1000, 0, pos.x, pos.y, -0, 000, .0, 1.0);
                    else if (brush == "black_hole")
                        addGalaxy(space, 0, 10000, 100000000000, 1000, 0, pos.x, pos.y, -0, 000, .0, 1.0);
                    else if (brush == "cluster100")
                        addGalaxy(space, 100, 1000, 1000, 100, 1, pos.x, pos.y, 0, 000, 0.0, 1.0);
                    else if (brush == "cluster1k")
                        addGalaxy(space, 1000, 1000, 1000, 1000, 1, pos.x, pos.y, 0, 000, 0.0, 1.0);
                    else if (brush == "cluster10k")
                        addGalaxy(space, 10000, 1000, 1000, 10000, 1, pos.x, pos.y, 0, 000, 0.0, 1.0);
                    else {
                        // Check if it's a saved selection
                        const GUI::SavedSelection* savedSel = gui.getSavedSelection(brush);
                        if (savedSel != nullptr) {
                            // Place the saved selection at the clicked position
                            for (const auto& bodyTemplate : savedSel->bodyTemplates) {
                                Body newBody = bodyTemplate;
                                newBody.position.x += pos.x;
                                newBody.position.y += pos.y;
                                newBody.selected = false;
                                newBody.shape.setFillColor(sf::Color::White);
                                space.addBody(newBody);
                            }
                        }
                    }
                }
            }

            // handling right mouse button presses and assumes multibody selection
            if(event.type == sf::Event::MouseButtonPressed and event.key.code == sf::Mouse::Right){
                gui.setSelecting(true);
                gui.setSelectionBegin(window.mapPixelToCoords(sf::Mouse::getPosition()));
            }

            // handling right mouse button releases to check whether selection is multi or single
            if(event.type == sf::Event::MouseButtonReleased and event.key.code == sf::Mouse::Right) {
                gui.setSelecting(false);

                // checks whether the mouse has dragged and therefore multiselect
                if(ImGui::IsMouseDragging(ImGuiMouseButton_Right)){
                    std::cout << "opened popup" << std::endl;
                    sf::Vector2f TL(std::min(gui.getSelectionBegin().x, gui.getSelectionEnd().x), std::min(gui.getSelectionBegin().y, gui.getSelectionEnd().y));
                    sf::Vector2f BR(std::max(gui.getSelectionBegin().x, gui.getSelectionEnd().x), std::max(gui.getSelectionBegin().y, gui.getSelectionEnd().y));

                    // loops through each body and checks whether its position is within the bounding box of the selection
                    for(auto& body : space.bodies){
                        body.selected = false;
                        if(body.position.x < BR.x and body.position.x > TL.x and body.position.y < BR.y and body.position.y > TL.y){
                            gui.getSelectedBodies().push_back(&body);
                            body.selected = true;
                        }
                    }

                    // checks whether the multiselect made any successful selections at all
                    if(!gui.getSelectedBodies().empty())
                        gui.setTriggerSelect(true);

                    if(paused) paused = false;
                }

            }



            // handling window resizing by changing the simulation view width to the new size
            if(event.type == sf::Event::Resized){
                std::cout << event.size.width << " " << event.size.height << std::endl;
                view.setSize(view.getSize().x * (event.size.width/window_width) , view.getSize().y * (event.size.height/window_height));
                window_width = event.size.width;
                window_height = event.size.height;
                gui.setWindowSize(window_width, window_height);
            }

        }


        // handling keyboard input for simulation space navigation ONLY if the window is focused
        if(gui.getFocus()) {
            // Q to zoom in
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                view.setSize(view.getSize().x * (1 - 1 * dt.asSeconds()), view.getSize().y * (1 - 1 * dt.asSeconds()));
            // E to zoom out
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                view.setSize(view.getSize().x * (1 + 1 * dt.asSeconds()), view.getSize().y * (1 + 1 * dt.asSeconds()));
            // W to move up
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                view.setCenter(view.getCenter().x, view.getCenter().y - speed * view.getSize().y * dt.asSeconds());
            // A to move left
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                view.setCenter(view.getCenter().x - speed * view.getSize().x * dt.asSeconds(), view.getCenter().y);
            // S to move down
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                view.setCenter(view.getCenter().x, view.getCenter().y + speed * view.getSize().y * dt.asSeconds());
            // D to move right
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                view.setCenter(view.getCenter().x + speed * view.getSize().x * dt.asSeconds(), view.getCenter().y);
        }


        // refreshes the view each frame as it might have changed due to simulation navigation
        window.setView(view);







        gui.update(window, dt);
        
        // Render GUI
        gui.render(window, space, selectionBox, iterations, dt, view);

        
        window.clear();


        // creates the selection box if the 'selecting' variable is active
        // the selectionbox is a white rectangular box with zero fill
        if(gui.isSelecting()){
            sf::Vector2f curmouse = window.mapPixelToCoords(sf::Mouse::getPosition());
            selectionBox.setFillColor(sf::Color::Transparent);
            selectionBox.setOutlineThickness(view.getSize().x/window_width);
            selectionBox.setOutlineColor(sf::Color::White);
            selectionBox.setSize(sf::Vector2f(curmouse.x-gui.getSelectionBegin().x, curmouse.y-gui.getSelectionBegin().y));
            selectionBox.setPosition(gui.getSelectionBegin());
            gui.setSelectionEnd(curmouse);
        }
        else if(gui.getSelectedBodies().empty()){
            selectionBox = sf::RectangleShape();
        }

        // SIMPLE RENDERING CODE
        // since arrays are static, we need to create a separate array
        // for each resolution
        sf::Texture tex1920; tex1920.create(1920,1080);
        sf::Texture tex0960; tex0960.create(1920/2,1080/2);
        sf::Texture tex0640; tex0640.create(1920/3,1080/3);
        sf::Texture tex0480; tex0480.create(1920/4,1080/4);
        sf::Texture tex0384; tex0384.create(1920/5,1080/5);
        sf::Texture tex0320; tex0320.create(1920/6,1080/6);
        sf::Texture tex0240; tex0240.create(1920/8,1080/8);
        sf::Texture tex0192; tex0192.create(1920/10,1080/10);
        sf::Texture tex0160; tex0160.create(1920/12,1080/12);
        sf::Texture tex0128; tex0128.create(1920/15,1080/15);


        // creates the main texture
        sf::Texture tex;
        tex.create(1920,1080);
        sf::Sprite sprite;
        sprite.setTexture(tex);

        // the simple rendering algorithm iterates through each planet, and converts its
        // coordinates to a location on a pixel grid by floor division of the coordinates
        // any planets with positions outside the grid are ignored
        if(gui.getSimpleRender()) {
            sf::Vector2f origin = sf::Vector2f(view.getCenter().x - view.getSize().x / 2,
                                               view.getCenter().y - view.getSize().y / 2);
            sf::Vector2f size = sf::Vector2f(view.getSize().x, view.getSize().y);
            pix = new sf::Uint8[1920 * 1080 * 4];
            memset(pix, 0, 1920 * 1080 * 4 * sizeof(sf::Uint8));

            double maxvel=-1;
            for(auto &body : space.bodies){
                sf::Vector2<double> pos = body.position;
                if (pos.x < origin.x or pos.x > origin.x + size.x) continue;
                if (pos.y < origin.y or pos.y > origin.y + size.y) continue;
                double vel = std::sqrt(std::pow(body.velocity.x,2) + std::pow(body.velocity.y,2));
                maxvel=std::max(vel,maxvel);
            }
            for (auto &body: space.bodies) {
                sf::Vector2<double> pos = body.position;

                if (pos.x < origin.x or pos.x > origin.x + size.x) continue;
                if (pos.y < origin.y or pos.y > origin.y + size.y) continue;

                double vel = std::sqrt(std::pow(body.velocity.x,2) + std::pow(body.velocity.y,2));
                body.shape.setFillColor(Body::convert_to_rgb(0,maxvel,vel));
                if (!gui.getUseColors()) body.shape.setFillColor(sf::Color::White);


                int gridx = (pos.x - origin.x) / (size.x / 1920.0);
                gridx -= gridx % gui.getScale();
                int gridy = (pos.y - origin.y) / (size.y / 1080.0);
                gridy -= gridy % gui.getScale();

                if(body.selected){
                    body.shape.setFillColor(sf::Color::White);
                }

                for (int i = 0; i < gui.getScale(); i++) {
                    for (int j = 0; j < gui.getScale(); j++) {
                        pix[4 * (((gridy + i) * 1920) + gridx + j) + 0] = body.shape.getFillColor().r;
                        pix[4 * (((gridy + i) * 1920) + gridx + j) + 1] = body.shape.getFillColor().g;
                        pix[4 * (((gridy + i) * 1920) + gridx + j) + 2] = body.shape.getFillColor().b;
                        pix[4 * (((gridy + i) * 1920) + gridx + j) + 3] = std::min(255,(int)pix[4 * (((gridy + i) * 1920) + gridx + j) + 3]+ (int)(255 * gui.getBrightness()));
                    }
                }
            }

            tex.update(pix);

            sprite.setOrigin(960, 540);
            sprite.setPosition(view.getCenter().x, view.getCenter().y);
            sprite.setScale((float) view.getSize().x / 1920.0f, (float) view.getSize().y / 1080.0f);

            window.draw(sprite);
        }
        else {
            // regular rendering mode simply loops through each object and 
            // renders its respective circleobject
            for(auto& body : space.bodies) {
                if(body.selected)
                    body.shape.setFillColor(sf::Color::White);
            }
            space.draw(window);
        }


        // update the quadTree, increment every force
        // and increment every position/velocity of the bodies
        space.updateTree(gui.getTreeBrightness());
        space.updateForces(false);
        space.updateBodies(dt.asMicroseconds() * gui.getTimescale() * paused);

        // optionally also render the quadTree
        if(gui.getRenderTree()) space.drawTree(window);

        // render the selectionbox 
        window.draw(selectionBox);
        ImGui::SFML::Render(window);
        window.display();

        // since the array is static and may contain pointers and therefore need to be 
        // freed from memory each frame of the program 
        if(gui.getSimpleRender()) delete[] pix;

        // restart the deltaclock
        dt = deltaClock.restart();
        if(paused) iterations++;
    }

    gui.shutdown();
    return 0;
}
