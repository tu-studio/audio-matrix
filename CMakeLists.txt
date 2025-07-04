# Set minimum cmake version that is required
cmake_minimum_required(VERSION 3.10)

execute_process(COMMAND git describe --dirty
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                OUTPUT_VARIABLE PROJECT_VERSION_FULL
                OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND git describe --tags --abbrev=0
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                OUTPUT_VARIABLE PROJECT_VERSION_SHORT
                OUTPUT_STRIP_TRAILING_WHITESPACE)

message(STATUS "Project version: ${PROJECT_VERSION_FULL}")

# Project name
set(PROJECT_NAME "audio-matrix")
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION_SHORT})

# Set cpp standard to c++20, otherwise std::numbers library will not be found
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_TESTS "Build tests" OFF)
option(VERSIONED_INSTALL "Install with version postfix and create symlinks" OFF)

# This disables the default behavior of adding all targets to the CTest dashboard, when adding libraries with the fetchcontent module
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)

# make version accessible
add_compile_definitions(VERSION="${PROJECT_VERSION_FULL}")

# Add all source files to file list
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/source/*.h)

if (VERSIONED_INSTALL)
    set(INSTALL_POSTFIX "-${PROJECT_VERSION_FULL}")
else()
    set(INSTALL_POSTFIX "")
endif()

message(STATUS "Install postfix: ${INSTALL_POSTFIX}")

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

# Add include directories for brew installed jack and liblo on macOS
if (APPLE)
    if (BUILD_ARCHITECTURE STREQUAL "arm64" OR CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64")
        target_include_directories(${PROJECT_NAME} PRIVATE /opt/homebrew/include)
        target_link_directories(${PROJECT_NAME} PRIVATE /opt/homebrew/lib)
    else ()
        target_include_directories(${PROJECT_NAME} PRIVATE /usr/local/include)
        target_link_directories(${PROJECT_NAME} PRIVATE /usr/local/lib)
    endif()
endif ()

include(FetchContent)

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG c9371de7836d113c0b14bfa15ca70f00ebb3ac6f #master branch as of 26.06.2025, update with proper version tag as soon as a new release happens
    )
FetchContent_GetProperties(yaml-cpp)


if(NOT yaml-cpp_POPULATED)
message(STATUS "Fetching yaml-cpp...")
    # Disable building of tools
    set(YAML_CPP_BUILD_TOOLS OFF CACHE INTERNAL "turn off yamlcpp targets")
    set(YAML_CPP_INSTALL OFF CACHE INTERNAL "turn off yamlcpp targets")
    set(YAML_CPP_FORMAT_SOURCE OFF CACHE INTERNAL "turn off yamlcpp targets")

    FetchContent_MakeAvailable(yaml-cpp)
endif()

# Link all of our libs together
target_link_libraries(${PROJECT_NAME} PUBLIC jack lo yaml-cpp::yaml-cpp)

# Set the output name of the executable
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}${INSTALL_POSTFIX}")

include(cmake/install.cmake)
add_subdirectory(systemd)

# Exclude from all also makes sure that the test libs are not installed
# https://stackoverflow.com/questions/64900981/how-do-you-prevent-cmake-from-install-ing-targets-from-within-projects-include
# but this is not a good solution, TODO: find a better solution
if (BUILD_TESTS)
    add_subdirectory(test)
endif()