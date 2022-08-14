# Define target as libname
set(TARGET lib2)

# Set Source files for lib
set(SOURCES
    lib2func1.cpp
    lib2func2.cpp
)

# Set Headers for lib
SET(HEADERS
    lib2func1.h
    lib2func2.h
)

# add library
add_library(${TARGET} ${SOURCES} ${HEADERS})

# users will inherit our library's include directory
target_include_directories(lib2
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(lib2
    lib1
)
