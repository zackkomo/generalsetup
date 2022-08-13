# 1. IDE/Code Editor
You can use VScode as a code editor for cpp, or you can use and IDE like CLION. When using an IDE most things are automated and debugging can be easier. An editor is more lightweight at you will need to set up a compiler and such on your own.

# 2. Compiler
On a windows device you can use MinGW, visual studio c++ or CLion. On a linux device you can install the cpp compiler from the package manager.

# 3. CMake
CMake is software meant to help in compilation. You can use CMakeFiles to specify dependencies and generate native tools to your system and compiler. Once installed, you can have a main function in your top directory:

~~~cpp
#include <iostream>

int main(){
    std::cout << "Hello World" << std::endl;
    return 0;
}
~~~

with a CMakeLists.txt in your top directory:
~~~cmake
cmake_minimum_required(VERSION 3.10)

# set the project name
project("cpp-game-engine")

# add the executable
add_executable(main main.cpp)
~~~

Next, create a build directory in the root folder called **build** and inside it run the following on linux:
~~~bash
cmake .. # point cmake to the top most CMakeLists.txt
make     # build the project
~~~
In the build folder an executable called **main** should appear. Running that is your main function.

On Windows, in the build folder run:
~~~
cmake --build .
~~~
In the build folder an executable called **main.exe** should appear. Running that is your main function.


# 3. VS code debugging

Install the following extensions:
- CMake
- CMakeTools
- C/C++

On the left pane find the "CMake" tile and click on it. A tab should open up and in the top right corner of the tab, click `Configure all projects`. If you want to change the CMAKE configurations, you can add commands to the `settings.json`. For example, to change the generator, add `"cmake.generator": "Unix Makefiles"` to the json to use Unix Makefiles.

In the tab, all the targets should appear. If you set a breakpoint in the code and then right click the target you want in the tab and select `Debug` it will now debug.

