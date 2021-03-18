cmake_minimum_required(VERSION 3.16)
project(SpaceImpact)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC src/main.cpp)

add_executable(space ${SRC})
target_link_libraries(space SDL2 SDL2_image SDL2_mixer SDL2_ttf)
target_compile_options(space PRIVATE -Wall -Wextra)

# Might use it later
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
