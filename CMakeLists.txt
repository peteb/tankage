cmake_minimum_required (VERSION 2.4)
project (SnailWail)

include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)

#CHECK_INCLUDE_FILE_CXX("tr1/memory" HAVE_TR1_MEMORY_H)

# check which smart pointers we can use (std, std::tr1, boost, ...)
# TODO: add test for dynamic_pointer_cast

SET(VERIFY_SMARTPTR_STD
  "class A {public: virtual ~A() {} }; class B : public A {}; int main() {std::shared_ptr<A> ptr1; std::weak_ptr<A> ptr2(ptr1); std::dynamic_pointer_cast<B>(ptr1); }")
SET(VERIFY_SMARTPTR_TR1
  "class A {public: virtual ~A() {} }; class B : public A {}; int main() {std::tr1::shared_ptr<A> ptr1; std::tr1::weak_ptr<A> ptr2(ptr1); std::tr1::dynamic_pointer_cast<B>(ptr1); }")

# TODO: only one VERIFY_SMARTPTR, send parameter for namespace
CHECK_CXX_SOURCE_COMPILES("#include <memory>\n ${VERIFY_SMARTPTR_STD}" HAVE_STD_SMARTPTRS)

if (NOT HAVE_STD_SMARTPTRS)
  CHECK_CXX_SOURCE_COMPILES("#include <tr1/memory>\n ${VERIFY_SMARTPTR_TR1}" HAVE_TR1_SMARTPTRS)

  if (NOT HAVE_TR1_SMARTPTRS)
	find_package(Boost REQUIRED)
	if (Boost_FOUND)
	  SET(HAVE_BOOST 1)
	endif (Boost_FOUND)
  endif (NOT HAVE_TR1_SMARTPTRS)
endif (NOT HAVE_STD_SMARTPTRS)



CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/config.h.in ${PROJECT_SOURCE_DIR}/src/config.h)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
  cmake_policy(SET CMP0011 NEW)
endif(COMMAND cmake_policy)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include_directories ("${PROJECT_SOURCE_DIR}/inc/" "${PROJECT_SOURCE_DIR}/src/")

add_subdirectory (src)

