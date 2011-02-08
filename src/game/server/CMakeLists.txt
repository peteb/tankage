# -*- cmake -*-

include_directories(${PROJECT_ROOT_DIR})

add_executable(snail-wail-server
  # common stuff
  ${GAME_ROOT_DIR}/common/system.cpp
  ${GAME_ROOT_DIR}/common/snails.cpp
  ${GAME_ROOT_DIR}/common/items.cpp
  ${GAME_ROOT_DIR}/common/projectile.cpp
  ${GAME_ROOT_DIR}/common/powerup.cpp
  ${GAME_ROOT_DIR}/common/texture_loader.cpp
  ${GAME_ROOT_DIR}/common/cactus.cpp
  
  # bad stuff
  ${GAME_ROOT_DIR}/client/particles.cpp
  
  # server specific
  main.cpp
  gameserver.cpp
)

target_link_libraries(snail-wail-server
	engine
)

