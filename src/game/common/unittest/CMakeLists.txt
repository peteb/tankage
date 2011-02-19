
if (NOT GTEST_FLAG) 
	return()
endif (NOT GTEST_FLAG)

include_directories(
  ${GTEST_INCLUDE_DIR}
  ${PROJECT_ROOT_DIR}/3pp
)

add_executable(conf_unittest EXCLUDE_FROM_ALL
  ../conf.cpp
  conf_unittest.cpp
)

target_link_libraries(conf_unittest
  ${GTEST_MAIN_LIBRARY}
  ${GTEST_LIBRARY}
  pthread
  ptrcfg 
)

add_dependencies(unittest conf_unittest)

