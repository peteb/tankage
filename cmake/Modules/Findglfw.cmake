# Locate the glfw library
# This module defines the following variables:
# GLFW_LIBRARY, the name of the library;
# GLFW_INCLUDE_DIR, where to find glfw include files.
# GLFW_FOUND, true if both the GLFW_LIBRARY and GLFW_INCLUDE_DIR have been found.
#
# To help locate the library and include file, you could define an environment variable called
# GLFW_ROOT which points to the root of the glfw library installation. This is pretty useful
# on a Windows platform.
#
#
# Usage example to compile an "executable" target to the glfw library:
#
# FIND_PACKAGE (glfw REQUIRED)
# INCLUDE_DIRECTORIES (${GLFW_INCLUDE_DIR})RARY_DIR})
# ADD_EXECUTABLE (executable ${EXECUTABLE_SRCS})
# TARGET_LINK_LIBRARIES (executable ${GLFW_LIBRARY})
#
# TODO:
# Allow the user to select to link to a shared library or to a static library.


# Macro to print some message to stdout, useful for debugging purpose.
MACRO(DBG_MSG _MSG)
  MESSAGE(STATUS "${CMAKE_CURRENT_LIST_FILE}(${CMAKE_CURRENT_LIST_LINE}): ${_MSG}")
ENDMACRO(DBG_MSG)

#Search for the include file...
FIND_PATH(GLFW_INCLUDE_DIR glfw.h DOC "Path to GLFW include directory."
  HINTS
  $ENV{GLFW_ROOT}
  PATH_SUFFIX include #For finding the include file under the root of the glfw expanded archive, typically on Windows.
  PATHS
  /usr/include/
  /usr/local/include/
  # By default headers are under GL subfolder
  /usr/include/GL
  /usr/local/include/GL	
  ${PROJECT_ROOT_DIR}/3pp/include/ # added by ptr
  ${PROJECT_ROOT_DIR}/3pp/include/GL/ # added by ptr

)
DBG_MSG("GLFW_INCLUDE_DIR = ${GLFW_INCLUDE_DIR}")

FIND_LIBRARY(GLFW_LIBRARY DOC "Absolute path to GLFW library."
  NAMES glfw
  HINTS
  $ENV{GLFW_ROOT}
  PATH_SUFFIXES lib/win32 #For finding the library file under the root of the glfw expanded archive, typically on Windows.
  PATHS
  /usr/local/lib
  /usr/lib
  ${PROJECT_ROOT_DIR}/3pp/lib/ # added by ptr
)
DBG_MSG("GLFW_LIBRARY = ${GLFW_LIBRARY}")

SET(GLFW_FOUND 0)
IF(GLFW_LIBRARY AND GLFW_INCLUDE_DIR)
  SET(GLFW_FOUND 1)
  DBG_MSG("GLFW found!")
ENDIF(GLFW_LIBRARY AND GLFW_INCLUDE_DIR)

