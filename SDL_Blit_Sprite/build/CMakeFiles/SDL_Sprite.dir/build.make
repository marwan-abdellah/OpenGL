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
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /opt/local/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Software/OpenGL/SDL_Blit_Sprite

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Software/OpenGL/SDL_Blit_Sprite/build

# Include any dependencies generated for this target.
include CMakeFiles/SDL_Sprite.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/SDL_Sprite.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SDL_Sprite.dir/flags.make

CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.o: CMakeFiles/SDL_Sprite.dir/flags.make
CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.o: ../SDL_Blit_Sprite.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /Software/OpenGL/SDL_Blit_Sprite/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.o -c /Software/OpenGL/SDL_Blit_Sprite/SDL_Blit_Sprite.cxx

CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Software/OpenGL/SDL_Blit_Sprite/SDL_Blit_Sprite.cxx > CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.i

CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Software/OpenGL/SDL_Blit_Sprite/SDL_Blit_Sprite.cxx -o CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.s

CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.o.requires:
.PHONY : CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.o.requires

CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.o.provides: CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.o.requires
	$(MAKE) -f CMakeFiles/SDL_Sprite.dir/build.make CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.o.provides.build
.PHONY : CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.o.provides

CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.o.provides.build: CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.o

# Object files for target SDL_Sprite
SDL_Sprite_OBJECTS = \
"CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.o"

# External object files for target SDL_Sprite
SDL_Sprite_EXTERNAL_OBJECTS =

SDL_Sprite: CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.o
SDL_Sprite: CMakeFiles/SDL_Sprite.dir/build.make
SDL_Sprite: /opt/local/lib/libSDLmain.a
SDL_Sprite: /opt/local/lib/libSDL.dylib
SDL_Sprite: CMakeFiles/SDL_Sprite.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable SDL_Sprite"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SDL_Sprite.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SDL_Sprite.dir/build: SDL_Sprite
.PHONY : CMakeFiles/SDL_Sprite.dir/build

CMakeFiles/SDL_Sprite.dir/requires: CMakeFiles/SDL_Sprite.dir/SDL_Blit_Sprite.cxx.o.requires
.PHONY : CMakeFiles/SDL_Sprite.dir/requires

CMakeFiles/SDL_Sprite.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SDL_Sprite.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SDL_Sprite.dir/clean

CMakeFiles/SDL_Sprite.dir/depend:
	cd /Software/OpenGL/SDL_Blit_Sprite/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Software/OpenGL/SDL_Blit_Sprite /Software/OpenGL/SDL_Blit_Sprite /Software/OpenGL/SDL_Blit_Sprite/build /Software/OpenGL/SDL_Blit_Sprite/build /Software/OpenGL/SDL_Blit_Sprite/build/CMakeFiles/SDL_Sprite.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SDL_Sprite.dir/depend
