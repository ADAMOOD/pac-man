cmake_minimum_required(VERSION 3.12)

project(pacman)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")


include_directories(include)

find_package(SDL2 REQUIRED)


set(SDL2_TTF_LIBRARIES "SDL2_ttf")

set(SOURCES
    src/main.c
    src/drawing.c
    src/menu.c
    src/map.c
    src/game.c
)


add_executable(pacman ${SOURCES})


target_include_directories(pacman PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})


target_link_libraries(pacman ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} m)