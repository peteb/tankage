
if (NOT GTEST_FLAG) 
	return()
endif (NOT GTEST_FLAG)

include_directories(
  ${GTEST_INCLUDE_DIR}
  ${PROJECT_ROOT_DIR}/3pp
)

add_executable(cfg_unittest EXCLUDE_FROM_ALL
  ${PROJECT_ROOT_DIR}/src/platform/cfg/cfg.cpp
  cfg_unittest.cpp
)

target_link_libraries(cfg_unittest
  ${GTEST_MAIN_LIBRARY}
  ${GTEST_LIBRARY}
  pthread
  ptrcfg 
)

add_dependencies(unittest cfg_unittest)

