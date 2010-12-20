# -*- mode: cmake -*-

cmake_minimum_required (VERSION 2.8) # FIXME: is this the earliest version with
                                     # support for PNG and JPEG?

find_package(OpenGL REQUIRED)
find_package(DevIL REQUIRED)

find_package(PNG) # for DevIL, not necessary if the symbols are already linked into DevIL..
find_package(JPEG)

add_library(graphics 
   gfx_subsystem.cpp
   sprite.cpp
   texture_loader.cpp
   render_list.cpp
   texture_fx.cpp
   sprite_culler.cpp
   color.cpp
)

if (HAVE_BOOST) # this seems to be necessary when generating VS projects
  include_directories(${Boost_INCLUDE_DIR})
endif (HAVE_BOOST)

include_directories(
   ${IL_INCLUDE_DIR}
   ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(graphics 
   ${OPENGL_LIBRARIES} 
   ${IL_LIBRARY} 
   ${PNG_LIBRARIES}
   ${JPEG_LIBRARIES}
)
