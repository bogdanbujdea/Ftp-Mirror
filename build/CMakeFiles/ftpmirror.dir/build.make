# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bogdan/projects/FtpMirror

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bogdan/projects/FtpMirror/build

# Include any dependencies generated for this target.
include CMakeFiles/ftpmirror.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ftpmirror.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ftpmirror.dir/flags.make

CMakeFiles/ftpmirror.dir/TcpClient.o: CMakeFiles/ftpmirror.dir/flags.make
CMakeFiles/ftpmirror.dir/TcpClient.o: ../TcpClient.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/bogdan/projects/FtpMirror/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ftpmirror.dir/TcpClient.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ftpmirror.dir/TcpClient.o -c /home/bogdan/projects/FtpMirror/TcpClient.cpp

CMakeFiles/ftpmirror.dir/TcpClient.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ftpmirror.dir/TcpClient.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/bogdan/projects/FtpMirror/TcpClient.cpp > CMakeFiles/ftpmirror.dir/TcpClient.i

CMakeFiles/ftpmirror.dir/TcpClient.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ftpmirror.dir/TcpClient.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/bogdan/projects/FtpMirror/TcpClient.cpp -o CMakeFiles/ftpmirror.dir/TcpClient.s

CMakeFiles/ftpmirror.dir/TcpClient.o.requires:
.PHONY : CMakeFiles/ftpmirror.dir/TcpClient.o.requires

CMakeFiles/ftpmirror.dir/TcpClient.o.provides: CMakeFiles/ftpmirror.dir/TcpClient.o.requires
	$(MAKE) -f CMakeFiles/ftpmirror.dir/build.make CMakeFiles/ftpmirror.dir/TcpClient.o.provides.build
.PHONY : CMakeFiles/ftpmirror.dir/TcpClient.o.provides

CMakeFiles/ftpmirror.dir/TcpClient.o.provides.build: CMakeFiles/ftpmirror.dir/TcpClient.o

CMakeFiles/ftpmirror.dir/FtpClient.o: CMakeFiles/ftpmirror.dir/flags.make
CMakeFiles/ftpmirror.dir/FtpClient.o: ../FtpClient.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/bogdan/projects/FtpMirror/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ftpmirror.dir/FtpClient.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ftpmirror.dir/FtpClient.o -c /home/bogdan/projects/FtpMirror/FtpClient.cpp

CMakeFiles/ftpmirror.dir/FtpClient.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ftpmirror.dir/FtpClient.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/bogdan/projects/FtpMirror/FtpClient.cpp > CMakeFiles/ftpmirror.dir/FtpClient.i

CMakeFiles/ftpmirror.dir/FtpClient.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ftpmirror.dir/FtpClient.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/bogdan/projects/FtpMirror/FtpClient.cpp -o CMakeFiles/ftpmirror.dir/FtpClient.s

CMakeFiles/ftpmirror.dir/FtpClient.o.requires:
.PHONY : CMakeFiles/ftpmirror.dir/FtpClient.o.requires

CMakeFiles/ftpmirror.dir/FtpClient.o.provides: CMakeFiles/ftpmirror.dir/FtpClient.o.requires
	$(MAKE) -f CMakeFiles/ftpmirror.dir/build.make CMakeFiles/ftpmirror.dir/FtpClient.o.provides.build
.PHONY : CMakeFiles/ftpmirror.dir/FtpClient.o.provides

CMakeFiles/ftpmirror.dir/FtpClient.o.provides.build: CMakeFiles/ftpmirror.dir/FtpClient.o

CMakeFiles/ftpmirror.dir/Exception.o: CMakeFiles/ftpmirror.dir/flags.make
CMakeFiles/ftpmirror.dir/Exception.o: ../Exception.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/bogdan/projects/FtpMirror/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ftpmirror.dir/Exception.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ftpmirror.dir/Exception.o -c /home/bogdan/projects/FtpMirror/Exception.cpp

CMakeFiles/ftpmirror.dir/Exception.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ftpmirror.dir/Exception.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/bogdan/projects/FtpMirror/Exception.cpp > CMakeFiles/ftpmirror.dir/Exception.i

CMakeFiles/ftpmirror.dir/Exception.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ftpmirror.dir/Exception.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/bogdan/projects/FtpMirror/Exception.cpp -o CMakeFiles/ftpmirror.dir/Exception.s

CMakeFiles/ftpmirror.dir/Exception.o.requires:
.PHONY : CMakeFiles/ftpmirror.dir/Exception.o.requires

CMakeFiles/ftpmirror.dir/Exception.o.provides: CMakeFiles/ftpmirror.dir/Exception.o.requires
	$(MAKE) -f CMakeFiles/ftpmirror.dir/build.make CMakeFiles/ftpmirror.dir/Exception.o.provides.build
.PHONY : CMakeFiles/ftpmirror.dir/Exception.o.provides

CMakeFiles/ftpmirror.dir/Exception.o.provides.build: CMakeFiles/ftpmirror.dir/Exception.o

CMakeFiles/ftpmirror.dir/main.o: CMakeFiles/ftpmirror.dir/flags.make
CMakeFiles/ftpmirror.dir/main.o: ../main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/bogdan/projects/FtpMirror/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ftpmirror.dir/main.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ftpmirror.dir/main.o -c /home/bogdan/projects/FtpMirror/main.cpp

CMakeFiles/ftpmirror.dir/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ftpmirror.dir/main.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/bogdan/projects/FtpMirror/main.cpp > CMakeFiles/ftpmirror.dir/main.i

CMakeFiles/ftpmirror.dir/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ftpmirror.dir/main.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/bogdan/projects/FtpMirror/main.cpp -o CMakeFiles/ftpmirror.dir/main.s

CMakeFiles/ftpmirror.dir/main.o.requires:
.PHONY : CMakeFiles/ftpmirror.dir/main.o.requires

CMakeFiles/ftpmirror.dir/main.o.provides: CMakeFiles/ftpmirror.dir/main.o.requires
	$(MAKE) -f CMakeFiles/ftpmirror.dir/build.make CMakeFiles/ftpmirror.dir/main.o.provides.build
.PHONY : CMakeFiles/ftpmirror.dir/main.o.provides

CMakeFiles/ftpmirror.dir/main.o.provides.build: CMakeFiles/ftpmirror.dir/main.o

# Object files for target ftpmirror
ftpmirror_OBJECTS = \
"CMakeFiles/ftpmirror.dir/TcpClient.o" \
"CMakeFiles/ftpmirror.dir/FtpClient.o" \
"CMakeFiles/ftpmirror.dir/Exception.o" \
"CMakeFiles/ftpmirror.dir/main.o"

# External object files for target ftpmirror
ftpmirror_EXTERNAL_OBJECTS =

ftpmirror: CMakeFiles/ftpmirror.dir/TcpClient.o
ftpmirror: CMakeFiles/ftpmirror.dir/FtpClient.o
ftpmirror: CMakeFiles/ftpmirror.dir/Exception.o
ftpmirror: CMakeFiles/ftpmirror.dir/main.o
ftpmirror: CMakeFiles/ftpmirror.dir/build.make
ftpmirror: CMakeFiles/ftpmirror.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ftpmirror"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ftpmirror.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ftpmirror.dir/build: ftpmirror
.PHONY : CMakeFiles/ftpmirror.dir/build

CMakeFiles/ftpmirror.dir/requires: CMakeFiles/ftpmirror.dir/TcpClient.o.requires
CMakeFiles/ftpmirror.dir/requires: CMakeFiles/ftpmirror.dir/FtpClient.o.requires
CMakeFiles/ftpmirror.dir/requires: CMakeFiles/ftpmirror.dir/Exception.o.requires
CMakeFiles/ftpmirror.dir/requires: CMakeFiles/ftpmirror.dir/main.o.requires
.PHONY : CMakeFiles/ftpmirror.dir/requires

CMakeFiles/ftpmirror.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ftpmirror.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ftpmirror.dir/clean

CMakeFiles/ftpmirror.dir/depend:
	cd /home/bogdan/projects/FtpMirror/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bogdan/projects/FtpMirror /home/bogdan/projects/FtpMirror /home/bogdan/projects/FtpMirror/build /home/bogdan/projects/FtpMirror/build /home/bogdan/projects/FtpMirror/build/CMakeFiles/ftpmirror.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ftpmirror.dir/depend
