add_executable(OrbitSim
    main.cpp
    Body.h
    Body.cpp
    QuadTree.h
    QuadTree.cpp
    Simulator.h
    Simulator.cpp
    globals.h
    ctpl.h
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -O3")

target_link_libraries(OrbitSim
    PUBLIC
        ImGui-SFML::ImGui-SFML
)
