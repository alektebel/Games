cmake_minimum_required(VERSION 3.24)
project(Learning)

# Find SDL2 and SDL2_image packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
set(CMAKE_CXX_STANDARD 17)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

# List your source files here (main.cpp and other .cpp files)
add_executable(Learning main.cpp)

# Link SDL2 and SDL2_image libraries to your executable
target_link_libraries(Learning SDL2 SDL2_ttf)
