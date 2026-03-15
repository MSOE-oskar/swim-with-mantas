cmake_minimum_required(VERSION 3.30)
project(SwimWithMantas)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES "src/*.cpp")
add_executable(SwimWithMantas ${SOURCES})
# Find OpenGL
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Link libraries
target_link_libraries(SwimWithMantas ${OPENGL_LIBRARIES} glad glfw3)