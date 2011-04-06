# -*- cmake -*-

include_directories(${PROJECT_ROOT_DIR}
  ${PROJECT_ROOT_DIR}/3pp 
)

add_executable(tankage
  # common stuff
  ${GAME_ROOT_DIR}/common/control.cpp
  ${GAME_ROOT_DIR}/common/texture_loader.cpp
  ${GAME_ROOT_DIR}/common/config.cpp

  ${GAME_ROOT_DIR}/server/gameserver.cpp  # shouldn't be used, but is needed for linking
  ${GAME_ROOT_DIR}/server/client_session.cpp
  ${GAME_ROOT_DIR}/server/tank.cpp
  ${GAME_ROOT_DIR}/server/events.cpp
  ${GAME_ROOT_DIR}/server/bullet.cpp
  ${GAME_ROOT_DIR}/server/map.cpp

  # client specific
  main.cpp
  gameclient.cpp
  tank_renderer.cpp
  text_renderer.cpp
  tank_info.cpp
  bullet_renderer.cpp
  map.cpp
  )
 
if (WIN32) 
  set(EXTRA_WINDOWS_LIBS "ws2_32.lib; winmm.lib")
endif (WIN32)

target_link_libraries(tankage
  platform_client
  ptrcfg
  ${EXTRA_WINDOWS_LIBS}
)

INSTALL(TARGETS tankage DESTINATION bin)

