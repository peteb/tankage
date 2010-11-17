# -*- mode: cmake -*-

find_package(OpenGL REQUIRED)

add_library(opengl
  device.cpp
  render_context.cpp
  texture.cpp
  )

if (HAVE_BOOST) # this seems to be necessary when generating VS projects
  include_directories(${Boost_INCLUDE_DIR})
endif (HAVE_BOOST)

include_directories(
  ${OPENGL_INCLUDE_DIR}
  )

target_link_libraries(opengl
  ${OPENGL_LIBRARIES} 
  )
