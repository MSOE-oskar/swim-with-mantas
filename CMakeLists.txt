cmake_minimum_required(VERSION 3.30)
project(SwimWithMantas)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES "src/*.cpp")
file(GLOB LIB_SOURCES "lib/*/*.cpp")
add_executable(SwimWithMantas ${SOURCES} ${LIB_SOURCES})
target_include_directories(SwimWithMantas PRIVATE lib)

# Find OpenGL
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Link libraries
target_link_libraries(SwimWithMantas ${OPENGL_LIBRARIES} glad glfw3)