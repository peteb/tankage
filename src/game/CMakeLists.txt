# -*- cmake -*-

add_executable(game
  main.cpp
  background.cpp
  )
  
target_link_libraries(game
  engine
  )
