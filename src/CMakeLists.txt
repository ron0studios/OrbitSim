add_executable(OrbitSim
    main.cpp
    Body.cpp
    QuadTree.cpp
    Simulator.cpp
)

target_include_directories(OrbitSim
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

# Find and link OpenMP
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    target_link_libraries(OrbitSim PUBLIC OpenMP::OpenMP_CXX)
endif()

target_link_libraries(OrbitSim
    PUBLIC
        ImGui-SFML::ImGui-SFML
)
