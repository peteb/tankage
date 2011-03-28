# -*- cmake -*-

include_directories(${PROJECT_ROOT_DIR}
  ${PROJECT_ROOT_DIR}/3pp 
)

add_executable(tankage
  # common stuff
  ${GAME_ROOT_DIR}/common/control.cpp
  ${GAME_ROOT_DIR}/common/texture_loader.cpp
  ${GAME_ROOT_DIR}/common/players.cpp
  ${GAME_ROOT_DIR}/common/net_error.cpp
  ${GAME_ROOT_DIR}/common/config.cpp

  ${GAME_ROOT_DIR}/server/gameserver.cpp  # shouldn't be used, but is needed for linking
  ${GAME_ROOT_DIR}/server/client_session.cpp
  ${GAME_ROOT_DIR}/server/tank.cpp

  # client specific
  main.cpp
  gameclient.cpp
  tank_renderer.cpp
  )
  
target_link_libraries(tankage
  engine
  ptrcfg
  ws2_32.lib
  winmm.lib
  )

INSTALL(TARGETS tankage DESTINATION bin)

