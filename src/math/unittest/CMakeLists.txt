
if (NOT GTEST_FLAG) 
	return()
endif (NOT GTEST_FLAG)

include_directories(
	${MATH_ROOT_DIR} 
	${GTEST_INCLUDE_DIR}
)

add_executable(math_unittest EXCLUDE_FROM_ALL
	math_unittest.cpp
)
target_link_libraries(math_unittest
	${GTEST_MAIN_LIBRARY}
	${GTEST_LIBRARY}
	pthread
)

