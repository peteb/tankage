# -*- cmake -*-

include_directories(${PROJECT_ROOT_DIR}
  ${PROJECT_ROOT_DIR}/3pp   
)

add_executable(tankage-server
  # common stuff
  ${GAME_ROOT_DIR}/common/texture_loader.cpp
  ${GAME_ROOT_DIR}/common/players.cpp
  ${GAME_ROOT_DIR}/common/net_error.cpp
  ${GAME_ROOT_DIR}/common/control.cpp
  ${GAME_ROOT_DIR}/common/config.cpp
  
  # bad stuff (shouldn't be needed)
  ${GAME_ROOT_DIR}/client/particles.cpp
  ${GAME_ROOT_DIR}/client/gameclient.cpp
  ${GAME_ROOT_DIR}/client/tank_renderer.cpp
  ${GAME_ROOT_DIR}/client/text_renderer.cpp

  # server specific
  main.cpp
  gameserver.cpp
  client_session.cpp
  tank.cpp
  
)

target_link_libraries(tankage-server
	engine
   ptrcfg
)

