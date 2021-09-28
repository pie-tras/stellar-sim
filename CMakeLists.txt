cmake_minimum_required(VERSION 3.0)

project("stellar_sim" VERSION 3.3.5 LANGUAGES C)

include_directories(lib/GLAD/include)
include_directories(lib/GLFW/include)
include_directories(lib/glm)
include_directories(stb_image)

add_executable(CMakeTarget glad.c sim.c)

target_link_libraries(CMakeTarget ${PROJECT_SOURCE_DIR}/lib/GLFW/lib64/libglfw3.a)