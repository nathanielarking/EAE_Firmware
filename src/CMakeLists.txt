# Set the source directory to the current directory
set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR})

# Recursively collect all .c and .h files in the source directory
file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.c ${SRC_DIR}/*.cpp ${SRC_DIR}/*.h)

# Define the name of the main project library using the project name with a "_lib" suffix
set(MAIN_PROJECT_LIBNAME ${PROJECT_NAME}_lib)

# Create a static library from the collected source files
add_library(${MAIN_PROJECT_LIBNAME} STATIC
    ${SOURCES}
)

# Add the main executable, specifying 'main.cpp' as the source file
add_executable(${PROJECT_NAME} main.cpp)

# Link the main executable with the static library created earlier
target_link_libraries(${PROJECT_NAME} ${MAIN_PROJECT_LIBNAME})

