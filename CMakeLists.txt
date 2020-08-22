cmake_minimum_required(VERSION 3.16)
project(SpaceImpact)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/sdl2")

find_package(SDL2 REQUIRED)

set(SRC src/main.cpp)

add_executable(space ${SRC})
target_include_directories(space PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(space ${SDL2_LIBRARIES})
target_compile_options(space PRIVATE -Wall -Wextra)

# Might use it later
# file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
