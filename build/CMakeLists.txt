# -*- mode: cmake -*-

cmake_minimum_required (VERSION 2.4)
project (tankage)

SET(CLIENT true)

if (DEFINED SERVER)
  UNSET(CLIENT)
endif (DEFINED SERVER)

if (DEFINED SERVER_AND_CLIENT)
  SET(SERVER true)
  SET(CLIENT true)
endif (DEFINED SERVER_AND_CLIENT)

include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)

#CHECK_INCLUDE_FILE_CXX("tr1/memory" HAVE_TR1_MEMORY_H)

# check which smart pointers we can use (std, std::tr1, boost, ...)

SET(VERIFY_SMARTPTR_STD
  "class A {public: virtual ~A() {} }; class B : public A {}; int main() {std::shared_ptr<A> ptr1; std::weak_ptr<A> ptr2(ptr1); std::dynamic_pointer_cast<B>(ptr1); }")
SET(VERIFY_SMARTPTR_TR1
  "class A {public: virtual ~A() {} }; class B : public A {}; int main() {std::tr1::shared_ptr<A> ptr1; std::tr1::weak_ptr<A> ptr2(ptr1); std::tr1::dynamic_pointer_cast<B>(ptr1); }")
SET(VERIFY_SMARTPTR_BOOST
  "class A {public: virtual ~A() {} }; class B : public A {}; int main() {boost::shared_ptr<A> ptr1; boost::weak_ptr<A> ptr2(ptr1); boost::dynamic_pointer_cast<B>(ptr1); }")

# TODO: only one VERIFY_SMARTPTR, send parameter for namespace
CHECK_CXX_SOURCE_COMPILES("#include <memory>\n ${VERIFY_SMARTPTR_STD}" HAVE_STD_SMARTPTRS)

if (NOT HAVE_STD_SMARTPTRS)
  CHECK_CXX_SOURCE_COMPILES("#include <tr1/memory>\n ${VERIFY_SMARTPTR_TR1}" HAVE_TR1_SMARTPTRS)

  if (NOT HAVE_TR1_SMARTPTRS)
	# check if the standard include file <memory> contains tr1; this is what MSVC does
	CHECK_CXX_SOURCE_COMPILES("#include <memory>\n ${VERIFY_SMARTPTR_TR1}" HAVE_TR1_IN_STD_SMARTPTRS)

	if (NOT HAVE_TR1_IN_STD_SMARTPTRS)
	  # no smart pointer support in either tr1 nor sc++l, fall back to boost
	  find_package(Boost REQUIRED)
	  if (Boost_FOUND)
		SET(HAVE_BOOST 1)

		# really verify that boost provides this
		CHECK_CXX_SOURCE_COMPILES("#include <boost/shared_ptr.hpp>\n#include <boost/weak_ptr.hpp>\n ${VERIFY_SMARTPTR_BOOST}" HAVE_BOOST_SMARTPTRS)
	  endif (Boost_FOUND)
	endif (NOT HAVE_TR1_IN_STD_SMARTPTRS)
  endif (NOT HAVE_TR1_SMARTPTRS)
endif (NOT HAVE_STD_SMARTPTRS)

IF (CMAKE_BUILD_TYPE)
  # convert to upper to ignore the case for the build type
  STRING(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
ENDIF (CMAKE_BUILD_TYPE)

IF(${CMAKE_BUILD_TYPE} MATCHES "DEBUG" AND CMAKE_COMPILER_IS_GNUCXX)
  ADD_DEFINITIONS(-g -Wall)
  SET(DEV 1)
ENDIF(${CMAKE_BUILD_TYPE} MATCHES "DEBUG" AND CMAKE_COMPILER_IS_GNUCXX)

set(PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR}/..)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_ROOT_DIR}/bin)

IF(${CMAKE_BUILD_TYPE} MATCHES "DEBUG")
  SET(RESOURCE_PATH "../data")
ELSE(${CMAKE_BUILD_TYPE} MATCHES "DEBUG")
  IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET(RESOURCE_PATH "${CMAKE_INSTALL_PREFIX}/share/tankage/data")
  ELSE(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET(RESOURCE_PATH "share/tankage/data")    
  ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
ENDIF(${CMAKE_BUILD_TYPE} MATCHES "DEBUG")

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
  cmake_policy(SET CMP0011 NEW)
endif(COMMAND cmake_policy)

set(CMAKE_MODULE_PATH "${PROJECT_ROOT_DIR}/cmake/Modules/")

include_directories ("${PROJECT_ROOT_DIR}/inc/" "${PROJECT_ROOT_DIR}/src/")

find_package(GTest)
find_package(Curl)
find_package(Box2D REQUIRED)

add_custom_target(unittest)

add_custom_target(run-unittest 
  ${PROJECT_ROOT_DIR}/cmake/Scripts/run-unittest.sh ${PROJECT_ROOT_DIR}
)
add_dependencies(run-unittest unittest)

add_subdirectory(${PROJECT_ROOT_DIR}/src CMakeFiles/Src)
add_subdirectory(${PROJECT_ROOT_DIR}/data CMakeFiles/Data)
add_subdirectory(${PROJECT_ROOT_DIR}/3pp/twitcurl CMakeFiles/Twitcurl)
add_subdirectory(${PROJECT_ROOT_DIR}/3pp/ptrcfg CMakeFiles/Ptrcfg)
add_subdirectory(${PROJECT_ROOT_DIR}/3pp/enet CMakeFiles/ENet)

add_custom_target(clean-gen
	rm -rf CMakeFiles CMakeCache.txt Makefile cmake_install.cmake ../bin 
	install_manifest.txt _CPack_Packages *.deb CPack*	
)

# Add CPack definitions for package generation on the current platform
include(Package)

