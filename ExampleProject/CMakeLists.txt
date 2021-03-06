# FEXTools
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project("ExampleProject.exe")

# Compiler flags
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
endif()

# Cache
if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set(CMAKE_C_FLAGS   "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

# Include directories
include_directories(/home/bruno/Dev/FEXTools/FEXTools)

# Source files
set (SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

# Define executable
add_executable(${PROJECT_NAME} ${SOURCES})

# CERN Root
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS RIO Net)
include(${ROOT_USE_FILE})

# FEXTools
add_subdirectory(/home/bruno/Dev/FEXTools/FEXTools /home/bruno/Dev/FEXTools/FEXTools/build) 

# Link Libraries
target_link_libraries(${PROJECT_NAME} FEXTools ${ROOT_LIBRARIES})

# Cache
set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")