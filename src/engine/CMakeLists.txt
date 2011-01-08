# -*- cmake -*-

add_library(engine
   portal.cpp
   glfw/wm.cpp
   glfw/input.cpp
   glfw/starter.cpp
   opengl/graphics.cpp
)

if (APPLE)
  find_package(Cocoa REQUIRED)
  include_directories(
    ${Cocoa_INCLUDE_DIR}
    )
endif (APPLE)


find_package(glfw REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(engine
  utils
  ${GLFW_LIBRARY} 
  ${Cocoa_LIBRARIES}
  ${OPENGL_LIBRARIES}
  )

