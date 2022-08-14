cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(crosslink VERSION 1.2)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# add the executable
add_executable(${PROJECT_NAME} main.cpp)

# add subdirectories
add_subdirectory(Lib1)
add_subdirectory(Lib2)

# add library to executable
target_link_libraries(${PROJECT_NAME}
  lib2
  lib1
)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h for versioning
target_include_directories(${PROJECT_NAME} PUBLIC
  "${PROJECT_BINARY_DIR}"
)
