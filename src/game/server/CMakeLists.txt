# -*- cmake -*-

include_directories(${PROJECT_ROOT_DIR})

add_executable(snail-wail-server
  # common stuff
  ${GAME_ROOT_DIR}/common/system.cpp
  ${GAME_ROOT_DIR}/common/actors.cpp
  ${GAME_ROOT_DIR}/common/tank.cpp
  ${GAME_ROOT_DIR}/common/projectiles.cpp
  ${GAME_ROOT_DIR}/common/projectile.cpp
  ${GAME_ROOT_DIR}/common/texture_loader.cpp
  ${GAME_ROOT_DIR}/common/players.cpp
  ${GAME_ROOT_DIR}/common/net_error.cpp
  ${GAME_ROOT_DIR}/common/control.cpp
  
  # bad stuff (shouldn't be needed)
  ${GAME_ROOT_DIR}/client/particles.cpp
  
  # server specific
  main.cpp
  gameserver.cpp
  client_session.cpp
)

target_link_libraries(snail-wail-server
	engine
)

