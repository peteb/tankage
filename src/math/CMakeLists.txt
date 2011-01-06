
set(MATH_ROOT_DIR ${PROJECT_ROOT_DIR}/src/math)

add_library(math
   vec2.cpp
   mat2.cpp
   rect.cpp
)

add_subdirectory(unittest)

