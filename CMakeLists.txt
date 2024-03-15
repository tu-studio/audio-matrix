# Set minimum cmake version that is required
cmake_minimum_required(VERSION 3.5)

# Project name
set(PROJECT_NAME "audio-matrix")
project(${PROJECT_NAME} VERSION 0.0.1)

# Set cpp standard to c++20, otherwise std::numbers library will not be found
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_TESTS "Build tests" OFF)

# This disables the default behavior of adding all targets to the CTest dashboard, when adding libraries with the fetchcontent module
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)

# Add all source files to file list
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/source/*.h)

# Add excecutables
add_executable(${PROJECT_NAME} ${SOURCES})

# Add include directories for all folders in the source
file(GLOB_RECURSE SUB_DIRS LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/source/*)
list(APPEND SUB_DIRS ${CMAKE_CURRENT_LIST_DIR}/source)
set(SUB_DIRS_RELATIVE "")

foreach (DIR ${SUB_DIRS})
    if (IS_DIRECTORY ${DIR})
        target_include_directories(${PROJECT_NAME} PRIVATE ${DIR})
        message(STATUS "Including directory ${DIR}")
        cmake_path(RELATIVE_PATH DIR BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/source OUTPUT_VARIABLE SUB_DIR_RELATIVE)
        if (NOT SUB_DIR_RELATIVE STREQUAL ".")
            list(APPEND SUB_DIRS_RELATIVE ${SUB_DIR_RELATIVE})
        endif()
    endif ()
endforeach ()


include(FetchContent)

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG 0.8.0
    )
FetchContent_GetProperties(yaml-cpp)


if(NOT yaml-cpp_POPULATED)
message(STATUS "Fetching yaml-cpp...")
    FetchContent_Populate(yaml-cpp)
    # Disable building of tools
    option(YAML_CPP_BUILD_TOOLS OFF)
    option(YAML_CPP_INSTALL OFF)
    option(YAML_CPP_FORMAT_SOURCE OFF)
    add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()

# Link all of our libs together
target_link_libraries(${PROJECT_NAME} PUBLIC jack lo yaml-cpp::yaml-cpp)

include(cmake/install.cmake)

# Exclude from all also makes sure that the test libs are not installed
# https://stackoverflow.com/questions/64900981/how-do-you-prevent-cmake-from-install-ing-targets-from-within-projects-include
if (BUILD_TESTS)
    add_subdirectory(test)
endif()