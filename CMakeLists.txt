cmake_minimum_required (VERSION 2.4)
project (SnailWail)

include (CheckIncludeFileCXX)

CHECK_INCLUDE_FILE_CXX("tr1/memory" HAVE_TR1_MEMORY_H)

# if (DEFINED HAVE_TR1_MEMORY_H)
#   MESSAGE("it does have tr1 memory: ${HAVE_TR1_MEMORY_H}")
# endif (DEFINED HAVE_TR1_MEMORY_H)

CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/config.h.in ${PROJECT_SOURCE_DIR}/src/config.h)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
  cmake_policy(SET CMP0011 NEW)
endif(COMMAND cmake_policy)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include_directories ("${PROJECT_SOURCE_DIR}/inc/" "${PROJECT_SOURCE_DIR}/src/")

add_subdirectory (src)

