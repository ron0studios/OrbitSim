include_directories(
    include
    tracy/public
)
option( TRACY_ENABLE "" OFF)
add_subdirectory(tracy)

add_executable(imgui-sfml-example
        main.cpp
        Body.h
        Body.cpp
        QuadTree.h
        QuadTree.cpp
        Simulator.h
        Simulator.cpp
        globals.h
        tracy/public/TracyClient.cpp
        tracy/public/tracy/Tracy.hpp
)


find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(imgui-sfml-example PUBLIC OpenMP::OpenMP_CXX)
endif()



# Yep, that's it!
target_link_libraries(imgui-sfml-example
  PUBLIC
    ImGui-SFML::ImGui-SFML
    Tracy::TracyClient
)

include(Install.cmake)
