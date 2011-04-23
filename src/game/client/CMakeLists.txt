# -*- cmake -*-

include_directories(${PROJECT_ROOT_DIR}
  ${PROJECT_ROOT_DIR}/3pp 
  ${BOX2D_INCLUDE_DIR}
)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  SET(TANKAGE_RESOURCE_FILE ${PROJECT_ROOT_DIR}/cmake/Scripts/tankage.rc)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

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
  ${GAME_ROOT_DIR}/server/world.cpp
  ${GAME_ROOT_DIR}/server/page.cpp
  ${GAME_ROOT_DIR}/server/resource.cpp

  # sets nice icon for the executable and main window =]
  ${TANKAGE_RESOURCE_FILE}

  # client specific
  main.cpp
  gameclient.cpp
  tank_renderer.cpp
  text_renderer.cpp
  tank_info.cpp
  bullet_renderer.cpp
  resource_renderer.cpp
  map.cpp
  )
 
if (WIN32) 
  set(EXTRA_WINDOWS_LIBS "ws2_32.lib;winmm.lib")
endif (WIN32)

target_link_libraries(tankage
  platform_client
  ptrcfg
  ${EXTRA_WINDOWS_LIBS}
  ${BOX2D_LIBRARY}
)

IF(APPLE)
  INSTALL(TARGETS tankage DESTINATION .)  
ELSE(APPLE)
  INSTALL(TARGETS tankage DESTINATION bin)
ENDIF(APPLE)
