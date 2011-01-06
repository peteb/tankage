
# Module to find GTest include directory and libraries.

# Unit tests are considered optional and in negative case user will be warned 
# but allowed to continue (user will be able to do make anyway).
# It is hard-coded to use static GTest libraries.

# Module defines
# GTEST_INCLUDE_DIR found by gtest/gtest.h
# GTEST_LIBRARY found by libgtest.a
# GTEST_MAIN_LIBRARY found by libgtest_main.a

# Module uses
# GTEST_ROOT environment variable as one of the hints

MACRO(DBG_MSG _LINE _MSG)
  MESSAGE(STATUS "${CMAKE_CURRENT_LIST_FILE}(${_LINE}): ${_MSG}")
ENDMACRO(DBG_MSG)

UNSET(GTEST_INCLUDE_DIR CACHE)
FIND_PATH(GTEST_INCLUDE_DIR gtest/gtest.h
	HINTS /usr/include/ /usr/local/include $ENV{GTEST_ROOT}/include
)
IF(GTEST_INCLUDE_DIR)
	DBG_MSG(${CMAKE_CURRENT_LIST_LINE} "GTEST_INCLUDE_DIR ${GTEST_INCLUDE_DIR}")
ELSE(GTEST_INCLUDE_DIR) 
	DBG_MSG(${CMAKE_CURRENT_LIST_LINE} "Failed to find gtest/gtest.h")
ENDIF(GTEST_INCLUDE_DIR)

UNSET(GTEST_LIBRARY CACHE)
FIND_LIBRARY(GTEST_LIBRARY libgtest.a
	HINTS /usr/lib /usr/local/lib $ENV{GTEST_ROOT}/lib
)
IF(GTEST_LIBRARY)
	DBG_MSG(${CMAKE_CURRENT_LIST_LINE} "GTEST_LIBRARY ${GTEST_LIBRARY}")
ELSE(GTEST_LIBRARY)
	DBG_MSG(${CMAKE_CURRENT_LIST_LINE} "Failed to find libgtest.a")
ENDIF(GTEST_LIBRARY)

UNSET(GTEST_MAIN_LIBRARY CACHE)
FIND_LIBRARY(GTEST_MAIN_LIBRARY libgtest_main.a
	HINTS /usr/lib /usr/local/lib $ENV{GTEST_ROOT}/lib
)
IF(GTEST_MAIN_LIBRARY)
	DBG_MSG(${CMAKE_CURRENT_LIST_LINE} "GTEST_MAIN_LIBRARY ${GTEST_MAIN_LIBRARY}")
ELSE(GTEST_MAIN_LIBRARY)
	DBG_MSG(${CMAKE_CURRENT_LIST_LINE} "Failed to find libgtest_main.a")
ENDIF(GTEST_MAIN_LIBRARY)

IF(GTEST_INCLUDE_DIR AND GTEST_LIBRARY AND GTEST_MAIN_LIBRARY)
	MESSAGE(STATUS "Found all required GTest stuff =]")
ELSE(GTEST_INCLUDE_DIR AND GTEST_LIBRARY AND GTEST_MAIN_LIBRARY)
	MESSAGE(STATUS "WARNING: Failed to find all or some of the required GTest stuff =[")
ENDIF(GTEST_INCLUDE_DIR AND GTEST_LIBRARY AND GTEST_MAIN_LIBRARY)

