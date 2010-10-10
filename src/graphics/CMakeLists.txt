

find_package(OpenGL)
#find_package()

add_library(graphics 
   gfx_subsystem.cpp
   sprite.cpp
   texture.cpp
   texture_loader.cpp
)

target_link_libraries(graphics ${OPENGL_LIBRARIES} ${DEVIL_LIBRARIES} IL png14 jpeg m z)