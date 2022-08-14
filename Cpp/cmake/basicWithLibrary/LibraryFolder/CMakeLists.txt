add_library(LibraryName libraryFunctions.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC
                    "${CMAKE_CURRENT_SOURCE_DIR}"
)
