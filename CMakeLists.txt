cmake_minimum_required (VERSION 2.4)
project (SnailWail)


#add_definitions("-arch x86_64")
#set(CMAKE_CXX_FLAGS "-arch x86_64")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include_directories ("${PROJECT_SOURCE_DIR}/inc/" "${PROJECT_SOURCE_DIR}/src/")

add_subdirectory (src)

