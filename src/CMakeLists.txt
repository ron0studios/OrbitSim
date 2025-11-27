# Common source files
set(ORBITSIM_SOURCES
    main.cpp
    Body.cpp
    QuadTree.cpp
    Simulator.cpp
    GUI.cpp
)

# Find OpenMP
find_package(OpenMP REQUIRED)

# Create Debug executable
add_executable(OrbitSim-Debug ${ORBITSIM_SOURCES})
target_include_directories(OrbitSim-Debug PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_options(OrbitSim-Debug PRIVATE ${DEBUG_FLAGS})
target_link_libraries(OrbitSim-Debug PUBLIC ImGui-SFML::ImGui-SFML)
if(OpenMP_CXX_FOUND)
    target_link_libraries(OrbitSim-Debug PUBLIC OpenMP::OpenMP_CXX)
endif()

# Create Release executable
add_executable(OrbitSim-Release ${ORBITSIM_SOURCES})
target_include_directories(OrbitSim-Release PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_options(OrbitSim-Release PRIVATE ${RELEASE_FLAGS})
target_link_libraries(OrbitSim-Release PUBLIC ImGui-SFML::ImGui-SFML)
if(OpenMP_CXX_FOUND)
    target_link_libraries(OrbitSim-Release PUBLIC OpenMP::OpenMP_CXX)
endif()

# Create convenience aliases
add_custom_target(debug DEPENDS OrbitSim-Debug)
add_custom_target(release DEPENDS OrbitSim-Release)
add_custom_target(all-builds DEPENDS OrbitSim-Debug OrbitSim-Release)
