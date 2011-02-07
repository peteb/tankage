# -*- cmake -*-

include_directories(${PROJECT_ROOT_DIR})

add_executable(snail-wail-server
  # common stuff
  ${GAME_ROOT_DIR}/common/system.cpp
  # client specific
  main.cpp
  gameserver.cpp
)

target_link_libraries(snail-wail-server
	engine
)

