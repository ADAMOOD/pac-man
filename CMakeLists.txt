cmake_minimum_required(VERSION 3.12)

project(pacman)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")


include_directories(include)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)


set(SOURCES
    src/main.c
)


add_executable(pacman ${SOURCES})


target_include_directories(pacman PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})


target_link_libraries(pacman ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} m)