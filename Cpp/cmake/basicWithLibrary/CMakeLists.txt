cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(BWL VERSION 1.2)

# Give option to not use/compile library. Default to on.
option(USE_LIBNAME "Use libraryName" ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(BWLConfig.h.in BWLConfig.h)

# add the executable
add_executable(${PROJECT_NAME} main.cpp)

# point this CMake file to a directory including others
if(USE_LIBNAME)
  add_subdirectory(LibraryFolder)
  list(APPEND EXTRA_LIBS LibraryName)

  #  list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
endif()

# add library to executable
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h for versioning
target_include_directories(${PROJECT_NAME} PUBLIC
                           "${PROJECT_BINARY_DIR}"
                          ${EXTRA_INCLUDES}
                           )
