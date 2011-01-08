# -*- cmake -*-

add_executable(engine
   interface.cpp
   portal.cpp
   modules/glfw.cpp
)

if (APPLE)
  find_package(Cocoa REQUIRED)
  include_directories(
    ${Cocoa_INCLUDE_DIR}
    )
  
#  target_link_libraries(snail-wail 
#    ${Cocoa_LIBRARIES}
#    )
endif (APPLE)


find_package(glfw REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(engine
  utils
  ${GLFW_LIBRARY} 
  ${Cocoa_LIBRARIES}
  ${OPENGL_LIBRARIES}
  )

