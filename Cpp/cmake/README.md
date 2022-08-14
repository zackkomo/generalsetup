# CMake
CMake is an "open-source, cross-platform family of tools designed to build, test and package software" mainly for `C/C++` but can also support Fortran, C# and CUDA. CMake is not a build system itself; it generates another system's build files. It is used in conjunction with native build environments such as `Make`, Qt Creator, Ninja, Android Studio, Apple's Xcode, and Microsoft Visual Studio.\
\
There will be __ project templates for examples on how a project might be structured.

- [CMake](#cmake)
  - [Basic template](#basic-template)
  - [Basic template with library](#basic-template-with-library)
  - [CMake useful commands](#cmake-useful-commands)
    - [<ins>Project Versioning</ins>](#insproject-versioningins)
    - [<ins>Specific C++ standard</ins>](#insspecific-c-standardins)
    - [<ins>Optionally add libraries</ins>](#insoptionally-add-librariesins)

## Basic template
___
The most basic structure is one like the `basic` project in this directory. A `CMakeLists.xtx` file is necessary in the `root` directory of the project, stating the cmake version requirements, the project name, and telling Cmake to create an executable from a source file.

```CMake
cmake_minimum_required(VERSION 3.10)

# set the project name
project(basicProject)

# add the executable
add_executable(executableName someExecutable.cpp)
```
If the project contains more than just one `.cpp` file, then those will need to be added to the executable created. This is also an opportunity to introduce some syntax to make the CMakeLists commands more dynamic. 

```CMake
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
```


In order to use CMake, you simply run CMake and pass in the path to the root directory. Since CMake will generate multiple files (i.e. makefiles) it is a good idea to create a folder for CMake to add all its supporting files.
```bash
# in project root 
mkdir build # folder for CMake to add its files to
cd build
cmake .. # pointing CMake one directory up, to the root directory
```
Now that CMake generated the makefiles, you can run make to compile and then run your executable
```bash
make
./basicProject
```

## Basic template with library
___
A variation of this can be seen in `./basicWithLibrary` with optional compilation implemented. If we want to make our own library for internal use in our project, we can do that using the `add_library()` CMake command. This is also useful to do when you have multiple folders made up of supporting code that you can organize in different libraries. All you need to do is add a `CMakeLists.txt` in the library directory with the `add_library()` CMake command, and use the `add_subdirectory()` in the higher level directory. You can also make the use and compilation of a library [optional](#insoptionally-add-librariesins).\
\
Upper level `CMakeLists.txt`
```CMake
cmake_minimum_required(VERSION 3.10)

# set the project name
project(BWL)

# add the executable
add_executable(${PROJECT_NAME} main.cpp)

# point this CMake file to a directory including others
# important to be done after add_executable() so that
# anything added to the ${PROJECT_NAME} in subdirectories
# can be added after it actually exists.
add_subdirectory(LibraryFolder)

# If it isn't possible to create the target before the
# library, you will have to include the library directory
# like so:
# target_include_directories(${PROJECT_NAME} PUBLIC
#                           "${PROJECT_SOURCE_DIR}/LibraryFolder"
#                            )

# link the library to the executable target
target_link_libraries(${PROJECT_NAME} PUBLIC LibraryName)

```
Lower level `CMakeLists.txt` in LibraryFolder
```CMake
add_library(LibraryName libraryFunctions.cpp)
# Add library directory to executable target's included directories.
# If you skip this command, you will need to include it in an upper 
# level CMakeLists.txt with the relative path. It is important that
# add_executable() have been already called in an upper level file 
# before the add_subdirectory() for this so that the following command
# uses an existing target.
target_include_directories(${PROJECT_NAME} PUBLIC
                    "${CMAKE_CURRENT_SOURCE_DIR}"
)
```


## CMake useful commands
___

### <ins>Project Versioning</ins>

An example can be seen in `./basicWithLibrary`. You can set the project version in `CMakeLists.txt` with
```CMake
project(ProjectName VERSION 1.0)
configure_file(TutorialConfig.h.in TutorialConfig.h)
target_include_directories(ProjectName PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
```
And then create a file called `TutorialConfig.h` in the directory with the source files containing
```c++
// the configured options and settings for ProjectName
#define ProjectName_VERSION_MAJOR @ProjectName_VERSION_MAJOR@
#define ProjectName_VERSION_MINOR @ProjectName_VERSION_MINOR@
```
And you can include `TutorialConfig.h` in your source files and have access to the version `ProjectName_VERSION_MAJOR`.`ProjectName_VERSION_MINOR` 

### <ins>Specific C++ standard</ins>
You can set the C++ standard in `CMakeLists.txt` with
```CMake
 # specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
```

### <ins>Optionally add libraries</ins>
An example can be seen in `./basicWithLibrary`. To do this, you will need to add a line in the project `CMakeLists.txt` declaring the option and providing a default value with `option()`. You will also need a config header declared after the option with `configure_file()` to parse whether the library was added to your source code.
```CMake
...
# set the project name and version
project(BWL VERSION 1.2)

# Give option to not use/compile library. Default to on.
option(USE_LIBNAME "Use libraryName" ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(BWLConfig.h.in BWLConfig.h)

# add the executable
add_executable(${PROJECT_NAME} main.cpp)
...

# point this CMake file to a directory including others conditionally
# to a list
if(USE_LIBNAME)
  add_subdirectory(LibraryFolder)
  list(APPEND EXTRA_LIBS LibraryName)
  # if  target_include_directories() using "${CMAKE_CURRENT_SOURCE_DIR}"
  # is NOT used in the lower level CMakeLists.txt, you need to include
  # the path as well with this list:
  # list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
endif()

# add conditional library list to executable. If the list is empty,
# no libraries will be added through EXTRA_LIBS
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})

# add conditional library directories list to executable. If the list is empty,
# no directories will be added through EXTRA_INCLUDES
target_include_directories(${PROJECT_NAME} PUBLIC
                          ${EXTRA_INCLUDES}
                           )

```
Next, add the option in the `.h.in` with this line:
```CMake
#cmakedefine USE_LIBNAME
```
Finally, in your source files you can use the following:
```c++
// make sure to import this first
#include "BWLConfig.h"
#ifdef USE_LIBNAME
#include "libraryName.h"
#endif
.... // somewhere in your source code
#ifdef USE_LIBNAME
    // use library functions
#else
    // don't use library functions
#endif
```

You can choose whether the library is used with the following:
```bash
# in build directory
cmake .. # uses default option setting
cmake .. -DUSE_LIBNAME=ON # sets to using library
cmake .. -DUSE_LIBNAME=ON # sets to not using library
make
```