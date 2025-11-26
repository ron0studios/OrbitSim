# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/ron0/development/cpp-projects/OrbitSim/build/_deps/imgui-sfml-src")
  file(MAKE_DIRECTORY "/home/ron0/development/cpp-projects/OrbitSim/build/_deps/imgui-sfml-src")
endif()
file(MAKE_DIRECTORY
  "/home/ron0/development/cpp-projects/OrbitSim/build/_deps/imgui-sfml-build"
  "/home/ron0/development/cpp-projects/OrbitSim/build/_deps/imgui-sfml-subbuild/imgui-sfml-populate-prefix"
  "/home/ron0/development/cpp-projects/OrbitSim/build/_deps/imgui-sfml-subbuild/imgui-sfml-populate-prefix/tmp"
  "/home/ron0/development/cpp-projects/OrbitSim/build/_deps/imgui-sfml-subbuild/imgui-sfml-populate-prefix/src/imgui-sfml-populate-stamp"
  "/home/ron0/development/cpp-projects/OrbitSim/build/_deps/imgui-sfml-subbuild/imgui-sfml-populate-prefix/src"
  "/home/ron0/development/cpp-projects/OrbitSim/build/_deps/imgui-sfml-subbuild/imgui-sfml-populate-prefix/src/imgui-sfml-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/ron0/development/cpp-projects/OrbitSim/build/_deps/imgui-sfml-subbuild/imgui-sfml-populate-prefix/src/imgui-sfml-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/ron0/development/cpp-projects/OrbitSim/build/_deps/imgui-sfml-subbuild/imgui-sfml-populate-prefix/src/imgui-sfml-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
