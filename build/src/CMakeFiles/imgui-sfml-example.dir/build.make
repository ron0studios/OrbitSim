# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ron0/imgui-sfml-fetchcontent

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ron0/imgui-sfml-fetchcontent/build

# Include any dependencies generated for this target.
include src/CMakeFiles/imgui-sfml-example.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/imgui-sfml-example.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/imgui-sfml-example.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/imgui-sfml-example.dir/flags.make

src/CMakeFiles/imgui-sfml-example.dir/main.cpp.o: src/CMakeFiles/imgui-sfml-example.dir/flags.make
src/CMakeFiles/imgui-sfml-example.dir/main.cpp.o: /home/ron0/imgui-sfml-fetchcontent/src/main.cpp
src/CMakeFiles/imgui-sfml-example.dir/main.cpp.o: src/CMakeFiles/imgui-sfml-example.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ron0/imgui-sfml-fetchcontent/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/imgui-sfml-example.dir/main.cpp.o"
	cd /home/ron0/imgui-sfml-fetchcontent/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/imgui-sfml-example.dir/main.cpp.o -MF CMakeFiles/imgui-sfml-example.dir/main.cpp.o.d -o CMakeFiles/imgui-sfml-example.dir/main.cpp.o -c /home/ron0/imgui-sfml-fetchcontent/src/main.cpp

src/CMakeFiles/imgui-sfml-example.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/imgui-sfml-example.dir/main.cpp.i"
	cd /home/ron0/imgui-sfml-fetchcontent/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ron0/imgui-sfml-fetchcontent/src/main.cpp > CMakeFiles/imgui-sfml-example.dir/main.cpp.i

src/CMakeFiles/imgui-sfml-example.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/imgui-sfml-example.dir/main.cpp.s"
	cd /home/ron0/imgui-sfml-fetchcontent/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ron0/imgui-sfml-fetchcontent/src/main.cpp -o CMakeFiles/imgui-sfml-example.dir/main.cpp.s

# Object files for target imgui-sfml-example
imgui__sfml__example_OBJECTS = \
"CMakeFiles/imgui-sfml-example.dir/main.cpp.o"

# External object files for target imgui-sfml-example
imgui__sfml__example_EXTERNAL_OBJECTS =

src/imgui-sfml-example: src/CMakeFiles/imgui-sfml-example.dir/main.cpp.o
src/imgui-sfml-example: src/CMakeFiles/imgui-sfml-example.dir/build.make
src/imgui-sfml-example: _deps/imgui-sfml-build/libImGui-SFML.so
src/imgui-sfml-example: _deps/sfml-build/lib/libsfml-graphics.so.2.5.1
src/imgui-sfml-example: _deps/sfml-build/lib/libsfml-window.so.2.5.1
src/imgui-sfml-example: _deps/sfml-build/lib/libsfml-system.so.2.5.1
src/imgui-sfml-example: /usr/lib/libGL.so
src/imgui-sfml-example: src/CMakeFiles/imgui-sfml-example.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/ron0/imgui-sfml-fetchcontent/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable imgui-sfml-example"
	cd /home/ron0/imgui-sfml-fetchcontent/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/imgui-sfml-example.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/imgui-sfml-example.dir/build: src/imgui-sfml-example
.PHONY : src/CMakeFiles/imgui-sfml-example.dir/build

src/CMakeFiles/imgui-sfml-example.dir/clean:
	cd /home/ron0/imgui-sfml-fetchcontent/build/src && $(CMAKE_COMMAND) -P CMakeFiles/imgui-sfml-example.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/imgui-sfml-example.dir/clean

src/CMakeFiles/imgui-sfml-example.dir/depend:
	cd /home/ron0/imgui-sfml-fetchcontent/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ron0/imgui-sfml-fetchcontent /home/ron0/imgui-sfml-fetchcontent/src /home/ron0/imgui-sfml-fetchcontent/build /home/ron0/imgui-sfml-fetchcontent/build/src /home/ron0/imgui-sfml-fetchcontent/build/src/CMakeFiles/imgui-sfml-example.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/CMakeFiles/imgui-sfml-example.dir/depend
