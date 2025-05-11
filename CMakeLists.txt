# Set the minimum version of CMake that can be used
cmake_minimum_required(VERSION 3.27)

# Set the project name and version, and specify it is written in C++
project(Minesweeper VERSION 1.0
        DESCRIPTION "C++ credit project"
        LANGUAGES CXX)

# Set the C++ standard to C++23 and require it, disable compiler extensions
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the source file
set(SOURCES src/main.cpp
        src/main.hpp
        src/generator.cpp
        src/generator.hpp
        src/cell.cpp
        src/cell.hpp
        src/field.cpp
        src/field.hpp
        src/technic.cpp
        src/technic.hpp
        src/solver.cpp
        src/solver.hpp
        src/player.cpp
        src/player.hpp
        src/settings.cpp
        src/settings.hpp)

# Look for headers in the `include` directory
include_directories(include)

# Create executable file 
add_executable(${PROJECT_NAME} ${SOURCES})

# Link used libraries (only SFML in this case)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
