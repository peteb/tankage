# -*- cmake -*-

#add_subdirectory(graphics)
#add_subdirectory(physics)
#add_subdirectory(utils)
#add_subdirectory(opengl)
add_subdirectory(utils)
add_subdirectory(engine)


#add_subdirectory(game)



if (HAVE_BOOST)
  include_directories(${Boost_INCLUDE_DIR})
  target_link_libraries(snail-wail ${Boost_LIBRARIES})
endif (HAVE_BOOST)

include_directories(
  ${GLFW_INCLUDE_DIR} 
  ${Cocoa_INCLUDE_DIR}
  )


