add_executable(OrbitSim
    main.cpp
    Body.cpp
    QuadTree.cpp
    Simulator.cpp
    GUI.cpp
)

target_include_directories(OrbitSim
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -O3")

target_link_libraries(OrbitSim
    PUBLIC
        ImGui-SFML::ImGui-SFML
)
