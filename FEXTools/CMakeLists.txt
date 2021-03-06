# FEXTools
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project("FEXTools")

# Compiler flags
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
endif()

# Cache
if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${warnings} -pedantic"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set(CMAKE_C_FLAGS   "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

# Include Directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Source files
set (SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FToolsClasses/FToolsObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FToolsClasses/DataSet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FToolsClasses/DataStack.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FToolsClasses/Fitter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FToolsClasses/Interpolator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FToolsClasses/FunctionPlotter.cpp
)

# Define library
add_library(${PROJECT_NAME} STATIC ${SOURCES})

# CERN Root
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS RIO Net)
include(${ROOT_USE_FILE})
target_link_libraries(FEXTools ${ROOT_LIBRARIES})

# Cache
set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")
