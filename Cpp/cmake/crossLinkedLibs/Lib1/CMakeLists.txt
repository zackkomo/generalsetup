# Define target as libname
set(TARGET lib1)

# Set Source files for lib
set(SOURCES
    ""
)

# Set Headers for lib
SET(HEADERS
    lib1func1.h
    lib1func2.h
)

# add library with only headers with interface
add_library(${TARGET} INTERFACE)

# users will inherit our library's include directory
target_include_directories(lib1
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(lib1 INTERFACE
    lib2
)