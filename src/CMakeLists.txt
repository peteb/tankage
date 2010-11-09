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

if (NOT HAVE_TR1_MEMORY_H)
  find_package(Boost REQUIRED)

  if (Boost_FOUND)
	set(HAVE_BOOST 1)
  endif (Boost_FOUND)
	
  include_directories(${Boost_INCLUDE_DIR})
  target_link_libraries(snail-wail ${Boost_LIBRARIES})
endif (NOT HAVE_TR1_MEMORY_H)

include_directories(
  ${GLFW_INCLUDE_DIR} 
  ${Cocoa_INCLUDE_DIR}
  )

target_link_libraries(snail-wail 
  graphics 
  physics 
  math
  ${GLFW_LIBRARY} 
  ${Cocoa_LIBRARIES}
  )


