
add_library(physics
   phys_subsystem.cpp
   body.cpp
   geom.cpp
)
if (HAVE_BOOST)
  include_directories(${Boost_INCLUDE_DIR})
endif (HAVE_BOOST)