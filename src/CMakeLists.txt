add_executable(imgui-sfml-example
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

# Yep, that's it!
target_link_libraries(imgui-sfml-example
  PUBLIC
    ImGui-SFML::ImGui-SFML
)

include(Install.cmake)
