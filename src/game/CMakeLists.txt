# -*- cmake -*-

add_executable(game
  main.cpp
  background.cpp
  snails.cpp
  control.cpp
  system.cpp
  items.cpp
  cactus.cpp
  projectile.cpp
  )
  
target_link_libraries(game
  engine
  )
