# -*- cmake -*-

if (DEFINED CLIENT)
  find_package(glfw REQUIRED)
  find_package(OpenGL REQUIRED)
  find_package(DevIL REQUIRED)
  find_package(PNG) # for DevIL, not necessary if the symbols are already linked into DevIL..
  find_package(JPEG)
  find_package(CURL)
endif (DEFINED CLIENT)


if (APPLE)
  find_package(Cocoa REQUIRED)
  include_directories(
    ${Cocoa_INCLUDE_DIR}
    )

  SET(PLATFORM_COCOA 1)
  SET(COCOA_ADDS cocoa/dialogs.m)
endif (APPLE)

if (DEFINED CLIENT)
  add_library(platform_client
    glfw/wm.cpp
    glfw/input.cpp
    opengl/graphics.cpp
    devil/image_loader.cpp
    enet/network.cpp
    curl/self_updater.cpp
    portal.cpp
    starter_client.cpp
    ${COCOA_ADDS}
    )
target_link_libraries(platform_client
  utils
  enet 
  ${GLFW_LIBRARY} 
  ${Cocoa_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${IL_LIBRARY} 
  ${PNG_LIBRARIES}
  ${JPEG_LIBRARIES}
  ${CURL_LIBRARIES}
  )
endif (DEFINED CLIENT)

if (DEFINED SERVER)
add_library(platform_server
  enet/network.cpp
  portal.cpp
  starter_server.cpp
  )
target_link_libraries(platform_server
  utils 
  enet
  )
endif (DEFINED SERVER)

include_directories(
   ${IL_INCLUDE_DIR}
   ${OPENGL_INCLUDE_DIR}
   ${PROJECT_ROOT_DIR}/3pp 
   ${PROJECT_ROOT_DIR}/3pp/include
   ${CURL_INCLUDE_DIR}
)

configure_file(${PROJECT_ROOT_DIR}/src/platform/config.h.in ${PROJECT_ROOT_DIR}/src/platform/config.h)

