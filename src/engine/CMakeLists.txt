# -*- cmake -*-

add_library(engine
   portal.cpp
   glfw/wm.cpp
   glfw/input.cpp
   glfw/starter.cpp
   opengl/graphics.cpp
   devil/image_loader.cpp
)

if (APPLE)
  find_package(Cocoa REQUIRED)
  include_directories(
    ${Cocoa_INCLUDE_DIR}
    )
endif (APPLE)


find_package(glfw REQUIRED)
find_package(OpenGL REQUIRED)
find_package(DevIL REQUIRED)

find_package(PNG) # for DevIL, not necessary if the symbols are already linked into DevIL..
find_package(JPEG)

include_directories(
   ${IL_INCLUDE_DIR}
   ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(engine
  utils
  ${GLFW_LIBRARY} 
  ${Cocoa_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${IL_LIBRARY} 
  ${PNG_LIBRARIES}
  ${JPEG_LIBRARIES}
  )

