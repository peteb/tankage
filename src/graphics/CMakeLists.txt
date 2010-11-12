

find_package(OpenGL REQUIRED)
find_package(DevIL REQUIRED)
find_package(png) # for DevIL
find_package(jpeg)

add_library(graphics 
   gfx_subsystem.cpp
   sprite.cpp
   texture.cpp
   texture_loader.cpp
   render_list.cpp
   material.cpp
   render_context.cpp
)

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