cmake_minimum_required(VERSION 3.10)

# set the project name
project(basicProject)

# set the variable SOURCES to be a list of the source files
set(SOURCES
    someExecutable.cpp
    anotherSourceFile.cpp
)

# add the executable and link its source files
# also name the executable the same as the project
add_executable(${PROJECT_NAME} ${SOURCES})

