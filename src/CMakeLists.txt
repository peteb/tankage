# -*- cmake -*-

#add_subdirectory(graphics)
#add_subdirectory(physics)
#add_subdirectory(utils)
#add_subdirectory(opengl)
add_subdirectory(utils)
add_subdirectory(platform)
add_subdirectory(game)

if (DEFINED SERVER)
add_subdirectory(tool)
endif (DEFINED SERVER)


if (HAVE_BOOST)
  include_directories(${Boost_INCLUDE_DIR})
  target_link_libraries(tankage ${Boost_LIBRARIES})
endif (HAVE_BOOST)

include_directories(
  ${GLFW_INCLUDE_DIR} 
  ${Cocoa_INCLUDE_DIR}
  )


