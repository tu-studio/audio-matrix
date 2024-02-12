# Set minimum cmake version that is required
cmake_minimum_required(VERSION 3.5)

# Project name
set(PROJECT_NAME "seamless_audio_matrix")
project(${PROJECT_NAME} VERSION 0.0.1)

# Set cpp standard to c++20, otherwise std::numbers library will not be found
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Adds a Debug flag to the compiler so that we can use it in the code as preprocessor directive
# In the FFT class we have lots of debug output that we only want to see when working on it
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

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

# Add example yaml config
add_subdirectory(examples)
message(STATUS "Config path: ${SIMPLE_WFS_MIXER_CONFIG_PATH}")
target_compile_definitions(${PROJECT_NAME} PUBLIC
    SIMPLE_WFS_MIXER_CONFIG_PATH="${SIMPLE_WFS_MIXER_CONFIG_PATH}"
)

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
  add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()

# Link all of our libs together
target_link_libraries(${PROJECT_NAME} PUBLIC jack lo yaml-cpp::yaml-cpp)

# This disables the default behavior of adding all targets to the CTest dashboard.
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
# enable ctest
include(CTest)

# Externally provided libraries
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    GIT_TAG v1.14.0)

FetchContent_Declare(benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    GIT_TAG v1.8.3)

# This command ensures that each of the named dependencies are made available to the project by the time it returns. If the dependency has already been populated the command does nothing. Otherwise, the command populates the dependency and then calls add_subdirectory() on the result.
FetchContent_MakeAvailable(googletest)

# For benchmark we want to set the BENCMARK_ENABLE_TESTING to OFF therefore we cannot use FetchContent_MakeAvailable()
# Check if population has already been performed
FetchContent_GetProperties(benchmark)
if(NOT benchmark_POPULATED)
    # Fetch the content using previously declared details
    FetchContent_Populate(benchmark)

    # Set custom variables, policies, etc.
    set(BENCHMARK_ENABLE_TESTING OFF)
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF)

    if (APPLE AND (BUILD_ARCHITECTURE STREQUAL "arm64" OR CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64"))
    set(HAVE_STD_REGEX ON)
    set(RUN_HAVE_STD_REGEX 1)
    endif()

    # Bring the populated content into the build
    add_subdirectory(${benchmark_SOURCE_DIR} ${benchmark_BINARY_DIR})

    # Supress warnings by making include directories system directories
    get_property(BENCHMARK_INCLUDE_DIRS TARGET benchmark PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
    target_include_directories(benchmark SYSTEM INTERFACE ${BENCHMARK_INCLUDE_DIRS})
endif()

# Make a test executable for every plugin
foreach(DIR IN LISTS SUB_DIRS_RELATIVE)

    # get all test cpp and header files
    file(GLOB_RECURSE TEST_SOURCES_${DIR} CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test/${DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/test/${DIR}/*.h)
    list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp)

    if (NOT TEST_SOURCES_${DIR})
        message(STATUS "No test files found for ${DIR}")
        continue()
    else ()
        message(STATUS "Found test files for ${DIR}")
        # Setup the test executable
        add_executable(Test_${DIR} ${TEST_SOURCES_${DIR}} ${SOURCES})
    endif ()

    # Add include directories for all folders in test
    file(GLOB_RECURSE TEST_DIRS_${DIR} LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR}/test/${DIR}/*)
    list(APPEND TEST_DIRS_${DIR} ${CMAKE_CURRENT_LIST_DIR}/test/${DIR})

    foreach (TEST_DIR IN LISTS TEST_DIRS_${DIR})
        if (IS_DIRECTORY ${TEST_DIR})
            message(STATUS "Including directory test ${TEST_DIR}")
            target_include_directories(Test_${DIR} PRIVATE ${TEST_DIR})
        endif ()
    endforeach ()

    foreach (SOURCE_DIR IN LISTS SUB_DIRS)
        if (IS_DIRECTORY ${SOURCE_DIR})
            target_include_directories(Test_${DIR} PRIVATE ${SOURCE_DIR})
            message(STATUS "Including directory test ${SOURCE_DIR}")
        endif ()
    endforeach ()

    # Link the test executable against gtest_main and google benchmark
    target_link_libraries(Test_${DIR} gtest_main benchmark::benchmark jack lo yaml-cpp)

    # include Loads and runs CMake code from the file given. Loads and runs CMake code from the file given.
    include(GoogleTest)
    # gtest_discover_tests will register a CTest test for each gtest and run them all in parallel with the rest of the Test.
    gtest_discover_tests(Test_${DIR} DISCOVERY_TIMEOUT 90)    
endforeach()