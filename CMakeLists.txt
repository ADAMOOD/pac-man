cmake_minimum_required(VERSION 3.12)

project(pacman)

# Přidání přepínačů překladače
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")

# Vyhledání knihovny SDL
find_package(SDL2)

# Vytvoření programu s názvem `hra`
add_executable(pacman main.c)

# Přidání adresářů s hlavičkovými soubory k programu (obdoba -I)
target_include_directories(pacman PRIVATE ${SDL2_INCLUDE_DIRS})

# Přilinkování knihoven k programu (obdoba -l)
target_link_libraries(pacman ${SDL2_LIBRARIES} m)
