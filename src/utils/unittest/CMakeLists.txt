
# Check GTest status and skip the file if negative
if (NOT GTEST_FLAG) 
	return()
endif (NOT GTEST_FLAG)

include_directories(
	${UTILS_ROOT_DIR} 
	${GTEST_INCLUDE_DIR}
)

add_executable(utils_unittest EXCLUDE_FROM_ALL
	utils_unittest.cpp
)

target_link_libraries(utils_unittest
	${GTEST_MAIN_LIBRARY}
	${GTEST_LIBRARY}
	pthread
	utils
)

