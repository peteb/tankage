# -*- cmake -*-

include_directories(${PROJECT_ROOT_DIR})

add_executable(tankage
  # common stuff
  ${GAME_ROOT_DIR}/common/actors.cpp
  ${GAME_ROOT_DIR}/common/tank.cpp
  ${GAME_ROOT_DIR}/common/control.cpp
  ${GAME_ROOT_DIR}/common/ai_control.cpp
  ${GAME_ROOT_DIR}/common/system.cpp
  ${GAME_ROOT_DIR}/common/projectiles.cpp
  ${GAME_ROOT_DIR}/common/projectile.cpp
  ${GAME_ROOT_DIR}/common/texture_loader.cpp
  ${GAME_ROOT_DIR}/common/players.cpp
  ${GAME_ROOT_DIR}/common/net_error.cpp

  ${GAME_ROOT_DIR}/server/gameserver.cpp  # shouldn't be used, but is needed for linking
  ${GAME_ROOT_DIR}/server/client_session.cpp

  # client specific
  particles.cpp
  background.cpp
  main.cpp
  gameclient.cpp
  )
  
target_link_libraries(tankage
  engine
  )

INSTALL(TARGETS tankage DESTINATION bin)

