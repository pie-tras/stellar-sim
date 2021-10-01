cmake_minimum_required(VERSION 3.10)

project(stellar_sim VERSION 0.1 LANGUAGES C)

include_directories(src)
include_directories(lib/GLAD/include)
include_directories(lib/GLFW/include)
include_directories(lib/glm)
include_directories(stb_image)

file(GLOB_RECURSE SRC_FILES "src/*.c")
file(GLOB_RECURSE HEADER_FILES "src/*.h")

add_executable(stellar_sim ${SRC_FILES} ${HEADER_FILES})

target_link_libraries(stellar_sim ${PROJECT_SOURCE_DIR}/lib/GLFW/lib64/libglfw3.a)