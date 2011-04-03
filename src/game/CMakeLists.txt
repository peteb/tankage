# -*- cmake -*-

set(GAME_ROOT_DIR ${PROJECT_ROOT_DIR}/src/game)

find_package(box2d REQUIRED)

add_subdirectory(client)
add_subdirectory(server)

