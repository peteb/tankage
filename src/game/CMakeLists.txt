# -*- cmake -*-

set(GAME_ROOT_DIR ${PROJECT_ROOT_DIR}/src/game)

if (NOT DEFINED SERVER) 
  add_subdirectory(client)
endif (NOT DEFINED SERVER)

add_subdirectory(server)

