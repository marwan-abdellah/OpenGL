# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/abdellah/Software/OpenGL/SDL_Mouse

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/abdellah/Software/OpenGL/SDL_Mouse/build

# Include any dependencies generated for this target.
include CMakeFiles/sdl_mouse.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sdl_mouse.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sdl_mouse.dir/flags.make

CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.o: CMakeFiles/sdl_mouse.dir/flags.make
CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.o: ../SDL_Mouse.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /home/abdellah/Software/OpenGL/SDL_Mouse/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.o -c /home/abdellah/Software/OpenGL/SDL_Mouse/SDL_Mouse.cxx

CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/abdellah/Software/OpenGL/SDL_Mouse/SDL_Mouse.cxx > CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.i

CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/abdellah/Software/OpenGL/SDL_Mouse/SDL_Mouse.cxx -o CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.s

CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.o.requires:
.PHONY : CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.o.requires

CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.o.provides: CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.o.requires
	$(MAKE) -f CMakeFiles/sdl_mouse.dir/build.make CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.o.provides.build
.PHONY : CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.o.provides

CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.o.provides.build: CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.o

# Object files for target sdl_mouse
sdl_mouse_OBJECTS = \
"CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.o"

# External object files for target sdl_mouse
sdl_mouse_EXTERNAL_OBJECTS =

sdl_mouse: CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.o
sdl_mouse: /usr/lib/x86_64-linux-gnu/libGLU.so
sdl_mouse: /usr/lib/libGL.so
sdl_mouse: /usr/lib/x86_64-linux-gnu/libSM.so
sdl_mouse: /usr/lib/x86_64-linux-gnu/libICE.so
sdl_mouse: /usr/lib/x86_64-linux-gnu/libX11.so
sdl_mouse: /usr/lib/x86_64-linux-gnu/libXext.so
sdl_mouse: /usr/lib/libglut.so
sdl_mouse: /usr/lib/x86_64-linux-gnu/libXmu.so
sdl_mouse: /usr/lib/x86_64-linux-gnu/libXi.so
sdl_mouse: /usr/lib/x86_64-linux-gnu/libSDLmain.a
sdl_mouse: /usr/lib/x86_64-linux-gnu/libSDL.so
sdl_mouse: CMakeFiles/sdl_mouse.dir/build.make
sdl_mouse: CMakeFiles/sdl_mouse.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable sdl_mouse"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sdl_mouse.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sdl_mouse.dir/build: sdl_mouse
.PHONY : CMakeFiles/sdl_mouse.dir/build

CMakeFiles/sdl_mouse.dir/requires: CMakeFiles/sdl_mouse.dir/SDL_Mouse.cxx.o.requires
.PHONY : CMakeFiles/sdl_mouse.dir/requires

CMakeFiles/sdl_mouse.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sdl_mouse.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sdl_mouse.dir/clean

CMakeFiles/sdl_mouse.dir/depend:
	cd /home/abdellah/Software/OpenGL/SDL_Mouse/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/abdellah/Software/OpenGL/SDL_Mouse /home/abdellah/Software/OpenGL/SDL_Mouse /home/abdellah/Software/OpenGL/SDL_Mouse/build /home/abdellah/Software/OpenGL/SDL_Mouse/build /home/abdellah/Software/OpenGL/SDL_Mouse/build/CMakeFiles/sdl_mouse.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sdl_mouse.dir/depend
