
add_subdirectory(graphics)
add_subdirectory(physics)

add_executable(snail-wail
   bullet.cpp
   game.cpp
   glfw_main.cpp
   hi_controller.cpp
   object_creator.cpp
   object_list.cpp
   player_entity.cpp
   rect.cpp
   snail.cpp
   vec2.cpp
   world.cpp
   scheduler.cpp
   object.cpp
)


find_package(Cocoa REQUIRED)
find_package(glfw REQUIRED)

include_directories(
   ${GLFW_INCLUDE_DIR} 
   ${Cocoa_INCLUDE_DIR}
)

target_link_libraries(snail-wail 
   graphics 
   physics 
   ${GLFW_LIBRARY} 
   ${Cocoa_LIBRARIES}
)
