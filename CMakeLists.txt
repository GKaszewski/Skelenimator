cmake_minimum_required(VERSION 3.2)
project(Skelenimator)

set(SOURCE
	src/main.c
	src/Bone.c
)

add_executable(Skelenimator ${SOURCE})

find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)

target_link_libraries(Skelenimator glfw -lGL)
target_link_libraries(Skelenimator GLEW)
target_link_libraries(Skelenimator SDL2)

target_include_directories(Skelenimator PUBLIC ${GLEW_INCLUDE_DIRS})