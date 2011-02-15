# -*- cmake -*-

include_directories(${PROJECT_ROOT_DIR})

add_executable(snail-wail
  # common stuff
  ${GAME_ROOT_DIR}/common/snails.cpp
  ${GAME_ROOT_DIR}/common/control.cpp
  ${GAME_ROOT_DIR}/common/ai_control.cpp
  ${GAME_ROOT_DIR}/common/system.cpp
  ${GAME_ROOT_DIR}/common/items.cpp
  ${GAME_ROOT_DIR}/common/cactus.cpp
  ${GAME_ROOT_DIR}/common/projectile.cpp
  ${GAME_ROOT_DIR}/common/powerup.cpp
  ${GAME_ROOT_DIR}/common/texture_loader.cpp
  ${GAME_ROOT_DIR}/common/players.cpp
  ${GAME_ROOT_DIR}/common/configuration.cpp

  # client specific
  particles.cpp
  background.cpp
  main.cpp
  gameclient.cpp
  )
  
target_link_libraries(snail-wail
  engine
  )

INSTALL(TARGETS snail-wail DESTINATION bin)

