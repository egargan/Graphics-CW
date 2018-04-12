cmake_minimum_required(VERSION 3.9)
project(Graphics-CW)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Put directories in project's 'include path'
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

# Declare the var 'SOURCE_FILES', and give it all relevant source filenames
set(SOURCE_FILES boilerplate.h Scene.cpp Model.h Water.cpp Water.h perlin.h Vec3.h Utility.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})