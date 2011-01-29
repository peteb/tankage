# -*- cmake -*-

add_executable(snail-wail
  main.cpp
  background.cpp
  snails.cpp
  control.cpp
  ai_control.cpp
  system.cpp
  items.cpp
  cactus.cpp
  projectile.cpp
  powerup.cpp
  particles.cpp
  texture_loader.cpp
  )
  
target_link_libraries(snail-wail
  engine
  )

INSTALL(TARGETS snail-wail DESTINATION bin)

