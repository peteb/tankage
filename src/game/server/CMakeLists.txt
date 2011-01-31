
include_directories(${PROJECT_ROOT_DIR})

add_executable(snail-wail-server
  main.cpp
)

target_link_libraries(snail-wail-server
	engine
)

