# -*- mode: cmake -*-

add_subdirectory(graphics)
add_subdirectory(physics)
add_subdirectory(math)
add_subdirectory(opengl)

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
  helmet.cpp
  )


if (APPLE)
  find_package(Cocoa REQUIRED)
  include_directories(
    ${Cocoa_INCLUDE_DIR}
    )
  
  target_link_libraries(snail-wail 
    ${Cocoa_LIBRARIES}
    )
endif (APPLE)


find_package(glfw REQUIRED)

if (HAVE_BOOST)
  include_directories(${Boost_INCLUDE_DIR})
  target_link_libraries(snail-wail ${Boost_LIBRARIES})
endif (HAVE_BOOST)

include_directories(
  ${GLFW_INCLUDE_DIR} 
  ${Cocoa_INCLUDE_DIR}
  )

target_link_libraries(snail-wail 
  graphics 
  opengl
  physics 
  math
  ${GLFW_LIBRARY} 
  ${Cocoa_LIBRARIES}
  )


