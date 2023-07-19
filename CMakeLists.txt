cmake_minimum_required(VERSION 3.25)
project(OrbitSim)

set(CMAKE_CXX_STANDARD 20)

#add_executable(OrbitSim main.cpp)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(OrbitSim main.cpp)
target_link_libraries(OrbitSim PRIVATE sfml-graphics sfml-audio sfml-window sfml-system sfml-network)
target_compile_features(OrbitSim PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET OrbitSim POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:OrbitSim> $<TARGET_FILE_DIR:OrbitSim> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS OrbitSim)

# Detect and add SFML
#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#find_package(SFML 2.6 REQUIRED system window graphics network audio)
#if(SFML_FOUND)
#    include_directories(${SFML_INCLUDE_DIR})
#    target_link_libraries(OrbitSim PRIVATE SFML::Graphics SFML::Audio SFML::Window SFML::System)
#endif()
