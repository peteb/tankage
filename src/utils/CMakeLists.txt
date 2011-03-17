# -*- cmake -*-

set(UTILS_ROOT_DIR ${PROJECT_ROOT_DIR}/src/utils)

add_library(utils
  vec.cpp
  mat.cpp
  rect.cpp
  color.cpp
  log.cpp
)

if (HAVE_BOOST)
  include_directories(${Boost_INCLUDE_DIR})
endif (HAVE_BOOST)

add_subdirectory(unittest)

