
include_directories(
  ${PROJECT_ROOT_DIR}/3pp
)

add_executable(tool
  tool.cc
)
target_link_libraries(tool
  enet
)

