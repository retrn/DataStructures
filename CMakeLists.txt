
cmake_minimum_required (VERSION 3.8)

project(datastructures)

# Add source to this project's executable.
file(GLOB_RECURSE SRC src/**.h src/**.c)
add_executable(DataStructures ${SRC})

target_include_directories(DataStructures 
	PRIVATE "src"
)
