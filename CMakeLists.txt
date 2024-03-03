cmake_minimum_required(VERSION 3.0)
project(MyProject)

# Set the path to wxWidgets installation directory
set(wxWidgets_ROOT_DIR /usr/include/wx-3.0)

# Find wxWidgets
find_package(wxWidgets REQUIRED)

# Include wxWidgets macros
include(${wxWidgets_USE_FILE})
include_directories(${wxWidgets_INCLUDE_DIRS})

# Add source files to your project
set(SOURCES
        client/src/main.cpp
)

# Add an executable target
add_executable(MyExecutable ${SOURCES})

# Link against wxWidgets libraries
target_link_libraries(MyExecutable ${wxWidgets_LIBRARIES})
