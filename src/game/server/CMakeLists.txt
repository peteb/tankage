# -*- cmake -*-

include_directories(${PROJECT_ROOT_DIR}
  ${PROJECT_ROOT_DIR}/3pp   
  ${BOX2D_INCLUDE_DIR}
)

add_executable(tankage-server
  # common stuff
  ${GAME_ROOT_DIR}/common/texture_loader.cpp
  ${GAME_ROOT_DIR}/common/control.cpp
  ${GAME_ROOT_DIR}/common/config.cpp
  
  # bad stuff (shouldn't be needed)
  ${GAME_ROOT_DIR}/client/particles.cpp
  ${GAME_ROOT_DIR}/client/gameclient.cpp
  ${GAME_ROOT_DIR}/client/tank_renderer.cpp
  ${GAME_ROOT_DIR}/client/text_renderer.cpp
  ${GAME_ROOT_DIR}/client/tank_info.cpp
  ${GAME_ROOT_DIR}/client/bullet_renderer.cpp
  ${GAME_ROOT_DIR}/client/map.cpp

  # server specific
  main.cpp
  gameserver.cpp
  client_session.cpp
  tank.cpp
  events.cpp
  bullet.cpp
  map.cpp
  world.cpp
)
 
if (WIN32) 
  set(EXTRA_WINDOWS_LIBS "ws2_32.lib; winmm.lib")
endif (WIN32)

target_link_libraries(tankage-server
  platform_server
  ptrcfg
  ${EXTRA_WINDOWS_LIBS}
  ${BOX2D_LIBRARY}
)

