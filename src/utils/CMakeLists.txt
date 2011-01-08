# -*- cmake -*-

add_library(utils
  vec.cpp
  mat.cpp
  rect.cpp
)

if (HAVE_BOOST)
  include_directories(${Boost_INCLUDE_DIR})
endif (HAVE_BOOST)
