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
CMAKE_SOURCE_DIR = /home/abdellah/Software/OpenGL/GLX_Simple

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/abdellah/Software/OpenGL/GLX_Simple/build

# Include any dependencies generated for this target.
include CMakeFiles/glx_sample.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/glx_sample.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/glx_sample.dir/flags.make

CMakeFiles/glx_sample.dir/GLX_Sample.cpp.o: CMakeFiles/glx_sample.dir/flags.make
CMakeFiles/glx_sample.dir/GLX_Sample.cpp.o: ../GLX_Sample.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/abdellah/Software/OpenGL/GLX_Simple/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/glx_sample.dir/GLX_Sample.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/glx_sample.dir/GLX_Sample.cpp.o -c /home/abdellah/Software/OpenGL/GLX_Simple/GLX_Sample.cpp

CMakeFiles/glx_sample.dir/GLX_Sample.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/glx_sample.dir/GLX_Sample.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/abdellah/Software/OpenGL/GLX_Simple/GLX_Sample.cpp > CMakeFiles/glx_sample.dir/GLX_Sample.cpp.i

CMakeFiles/glx_sample.dir/GLX_Sample.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/glx_sample.dir/GLX_Sample.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/abdellah/Software/OpenGL/GLX_Simple/GLX_Sample.cpp -o CMakeFiles/glx_sample.dir/GLX_Sample.cpp.s

CMakeFiles/glx_sample.dir/GLX_Sample.cpp.o.requires:
.PHONY : CMakeFiles/glx_sample.dir/GLX_Sample.cpp.o.requires

CMakeFiles/glx_sample.dir/GLX_Sample.cpp.o.provides: CMakeFiles/glx_sample.dir/GLX_Sample.cpp.o.requires
	$(MAKE) -f CMakeFiles/glx_sample.dir/build.make CMakeFiles/glx_sample.dir/GLX_Sample.cpp.o.provides.build
.PHONY : CMakeFiles/glx_sample.dir/GLX_Sample.cpp.o.provides

CMakeFiles/glx_sample.dir/GLX_Sample.cpp.o.provides.build: CMakeFiles/glx_sample.dir/GLX_Sample.cpp.o

# Object files for target glx_sample
glx_sample_OBJECTS = \
"CMakeFiles/glx_sample.dir/GLX_Sample.cpp.o"

# External object files for target glx_sample
glx_sample_EXTERNAL_OBJECTS =

glx_sample: CMakeFiles/glx_sample.dir/GLX_Sample.cpp.o
glx_sample: /usr/lib/x86_64-linux-gnu/libGLU.so
glx_sample: /usr/lib/libGL.so
glx_sample: /usr/lib/x86_64-linux-gnu/libSM.so
glx_sample: /usr/lib/x86_64-linux-gnu/libICE.so
glx_sample: /usr/lib/x86_64-linux-gnu/libX11.so
glx_sample: /usr/lib/x86_64-linux-gnu/libXext.so
glx_sample: /usr/lib/libglut.so
glx_sample: /usr/lib/x86_64-linux-gnu/libXmu.so
glx_sample: /usr/lib/x86_64-linux-gnu/libXi.so
glx_sample: /usr/lib/x86_64-linux-gnu/libSDLmain.a
glx_sample: /usr/lib/x86_64-linux-gnu/libSDL.so
glx_sample: CMakeFiles/glx_sample.dir/build.make
glx_sample: CMakeFiles/glx_sample.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable glx_sample"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/glx_sample.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/glx_sample.dir/build: glx_sample
.PHONY : CMakeFiles/glx_sample.dir/build

CMakeFiles/glx_sample.dir/requires: CMakeFiles/glx_sample.dir/GLX_Sample.cpp.o.requires
.PHONY : CMakeFiles/glx_sample.dir/requires

CMakeFiles/glx_sample.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/glx_sample.dir/cmake_clean.cmake
.PHONY : CMakeFiles/glx_sample.dir/clean

CMakeFiles/glx_sample.dir/depend:
	cd /home/abdellah/Software/OpenGL/GLX_Simple/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/abdellah/Software/OpenGL/GLX_Simple /home/abdellah/Software/OpenGL/GLX_Simple /home/abdellah/Software/OpenGL/GLX_Simple/build /home/abdellah/Software/OpenGL/GLX_Simple/build /home/abdellah/Software/OpenGL/GLX_Simple/build/CMakeFiles/glx_sample.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/glx_sample.dir/depend

