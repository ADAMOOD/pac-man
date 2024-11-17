cmake_minimum_required(VERSION 3.12)

project(pacman)

# Přidání přepínačů překladače
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")

# Specify the include directory
include_directories(include)

# Vyhledání knihovny SDL
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Source files
set(SOURCES
    src/main.c
)

# Add executable
add_executable(pacman ${SOURCES})

# Přidání adresářů s hlavičkovými soubory k programu (obdoba -I)
target_include_directories(pacman PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

# Přilinkování knihoven k programu (obdoba -l)
target_link_libraries(pacman ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} m)