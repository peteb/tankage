
include_directories(
	${MATH_ROOT_DIR} 
	${GTEST_INCLUDE_DIR}
)

add_executable(math_unittest
	math_unittest.cpp
)
target_link_libraries(math_unittest
	${GTEST_MAIN_LIBRARY}
	${GTEST_LIBRARY}
	pthread
)

