cmake_minimum_required(VERSION 3.12)

project(pacman)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")

include_directories(include)

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)  # Přidání SDL_mixer
# find_package(SDL2_image REQUIRED)
# find_package(SDL2_ttf REQUIRED)

set(SDL2_TTF_LIBRARIES "SDL2_ttf")
set(SDL2_IMAGE_LIBRARIES "SDL2_image")
set(SDL2_MIXER_LIBRARIES "SDL2_mixer")

set(SOURCES
    src/main.c
    src/drawing.c
    src/menu.c
    src/map.c
    src/game.c
    src/player.c
    src/uiInfo.c
    src/ghost.c
    src/universal.c
)

add_executable(pacman ${SOURCES})

target_include_directories(pacman PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})

# Přidání SDL2_mixer k propojení knihoven
target_link_libraries(pacman PRIVATE ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES} m)

#add_custom_target(valgrind
#   COMMAND valgrind --leak-check=full --track-origins=yes ./pacman
#   DEPENDS pacman
#   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#)