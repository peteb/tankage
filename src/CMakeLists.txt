# -*- Mode: cmake -*-

add_subdirectory(graphics)
add_subdirectory(physics)
add_subdirectory(math)

add_executable(snail-wail
  bullet.cpp
  game.cpp
  glfw_main.cpp
  hi_controller.cpp
  object_creator.cpp
  object_list.cpp
  player_entity.cpp
  snail.cpp
  world.cpp
  scheduler.cpp
  object.cpp
  cactus.cpp
  cactus_generator.cpp
  health_meter.cpp
  )


find_package(Cocoa REQUIRED)
find_package(glfw REQUIRED)
find_package(Boost REQUIRED)

include_directories(
  ${GLFW_INCLUDE_DIR} 
  ${Cocoa_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR}
  )

target_link_libraries(snail-wail 
  graphics 
  physics 
  math
  ${GLFW_LIBRARY} 
  ${Cocoa_LIBRARIES}
  ${Boost_LIBRARIES}
  )
