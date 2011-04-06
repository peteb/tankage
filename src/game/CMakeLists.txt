# -*- cmake -*-

set(GAME_ROOT_DIR ${PROJECT_ROOT_DIR}/src/game)

if (DEFINED CLIENT)
  add_subdirectory(client)
endif (DEFINED CLIENT)

if (DEFINED SERVER)
  add_subdirectory(server)
endif (DEFINED SERVER)

