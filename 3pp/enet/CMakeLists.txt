
include_directories(${PROJECT_ROOT_DIR}/3pp)

file(GLOB ENET_SOURCE_FILES "*.c")
add_library(enet
  ${ENET_SOURCE_FILES}
)

