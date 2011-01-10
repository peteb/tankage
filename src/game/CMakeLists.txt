# -*- cmake -*-

add_executable(game
  main.cpp
  background.cpp
  snails.cpp
  control.cpp
  system.cpp
  items.cpp
  projectiles.cpp
  )
  
target_link_libraries(game
  engine
  )
