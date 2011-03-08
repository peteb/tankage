
if (NOT GTEST_FLAG) 
	return()
endif (NOT GTEST_FLAG)

include_directories(
	${UTILS_ROOT_DIR} 
	${GTEST_INCLUDE_DIR}
)

add_executable(utils_unittest EXCLUDE_FROM_ALL
  mat_unittest.cpp
  vec_unittest.cpp
  rect_unittest.cpp
  ring_buffer_unittest.cpp     
)

target_link_libraries(utils_unittest
	${GTEST_MAIN_LIBRARY}
	${GTEST_LIBRARY}
	pthread
	utils
)

add_dependencies(unittest utils_unittest)

