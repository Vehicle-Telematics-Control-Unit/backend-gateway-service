# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /home/abbas/grad_proj_repos/backend-gateway-service

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/abbas/grad_proj_repos/backend-gateway-service/build

# Include any dependencies generated for this target.
include CMakeFiles/backend_gateway.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/backend_gateway.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/backend_gateway.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/backend_gateway.dir/flags.make

CMakeFiles/backend_gateway.dir/src/main.cpp.o: CMakeFiles/backend_gateway.dir/flags.make
CMakeFiles/backend_gateway.dir/src/main.cpp.o: /home/abbas/grad_proj_repos/backend-gateway-service/src/main.cpp
CMakeFiles/backend_gateway.dir/src/main.cpp.o: CMakeFiles/backend_gateway.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/abbas/grad_proj_repos/backend-gateway-service/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/backend_gateway.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/backend_gateway.dir/src/main.cpp.o -MF CMakeFiles/backend_gateway.dir/src/main.cpp.o.d -o CMakeFiles/backend_gateway.dir/src/main.cpp.o -c /home/abbas/grad_proj_repos/backend-gateway-service/src/main.cpp

CMakeFiles/backend_gateway.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/backend_gateway.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/abbas/grad_proj_repos/backend-gateway-service/src/main.cpp > CMakeFiles/backend_gateway.dir/src/main.cpp.i

CMakeFiles/backend_gateway.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/backend_gateway.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/abbas/grad_proj_repos/backend-gateway-service/src/main.cpp -o CMakeFiles/backend_gateway.dir/src/main.cpp.s

# Object files for target backend_gateway
backend_gateway_OBJECTS = \
"CMakeFiles/backend_gateway.dir/src/main.cpp.o"

# External object files for target backend_gateway
backend_gateway_EXTERNAL_OBJECTS =

backend_gateway: CMakeFiles/backend_gateway.dir/src/main.cpp.o
backend_gateway: CMakeFiles/backend_gateway.dir/build.make
backend_gateway: dependencies/http_client/libhttp_client.so
backend_gateway: dependencies/someipService/libsomeipservice.so
backend_gateway: /usr/lib/libcurl.so
backend_gateway: /home/abbas/grad_proj_repos/sand_box/vsomeip_containers/vsomeip-3.1.20.3/build/libvsomeip3.so
backend_gateway: /home/abbas/boost_1_58_0/boost_1_58_0/build/lib/libboost_system.so
backend_gateway: /home/abbas/boost_1_58_0/boost_1_58_0/build/lib/libboost_thread.so
backend_gateway: /home/abbas/boost_1_58_0/boost_1_58_0/build/lib/libboost_log.so
backend_gateway: /home/abbas/boost_1_58_0/boost_1_58_0/build/lib/libboost_chrono.so
backend_gateway: /home/abbas/boost_1_58_0/boost_1_58_0/build/lib/libboost_date_time.so
backend_gateway: /home/abbas/boost_1_58_0/boost_1_58_0/build/lib/libboost_atomic.so
backend_gateway: /home/abbas/boost_1_58_0/boost_1_58_0/build/lib/libboost_log_setup.so
backend_gateway: /home/abbas/boost_1_58_0/boost_1_58_0/build/lib/libboost_filesystem.so
backend_gateway: /home/abbas/boost_1_58_0/boost_1_58_0/build/lib/libboost_regex.so
backend_gateway: CMakeFiles/backend_gateway.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/abbas/grad_proj_repos/backend-gateway-service/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable backend_gateway"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/backend_gateway.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/backend_gateway.dir/build: backend_gateway
.PHONY : CMakeFiles/backend_gateway.dir/build

CMakeFiles/backend_gateway.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/backend_gateway.dir/cmake_clean.cmake
.PHONY : CMakeFiles/backend_gateway.dir/clean

CMakeFiles/backend_gateway.dir/depend:
	cd /home/abbas/grad_proj_repos/backend-gateway-service/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/abbas/grad_proj_repos/backend-gateway-service /home/abbas/grad_proj_repos/backend-gateway-service /home/abbas/grad_proj_repos/backend-gateway-service/build /home/abbas/grad_proj_repos/backend-gateway-service/build /home/abbas/grad_proj_repos/backend-gateway-service/build/CMakeFiles/backend_gateway.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/backend_gateway.dir/depend
