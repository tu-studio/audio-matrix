# Set minimum cmake version that is required
cmake_minimum_required(VERSION 3.14)

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
set(SUB_DIRS_RELATIVE "")

foreach (DIR ${SUB_DIRS})
    if (IS_DIRECTORY ${DIR})
        target_include_directories(${PROJECT_NAME} PRIVATE ${DIR})
        message(STATUS "Including directory ${DIR}")
        cmake_path(RELATIVE_PATH DIR BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/source OUTPUT_VARIABLE SUB_DIR_RELATIVE)
        list(APPEND SUB_DIRS_RELATIVE ${SUB_DIR_RELATIVE})
    endif ()
endforeach ()

message(STATUS "Found subdirectories: ${SUB_DIRS_RELATIVE}")
message(STATUS "Found source files: ${SUB_DIRS}")

# Link all of our libs together
target_link_libraries(${PROJECT_NAME} PUBLIC jack lo)

# We include(CTest) so that cmake can find the unit tests for which we use the GoogleTest framework.
include(CTest)

# load the module FetchContent, which enables us to download external resource
# in our case GoogleTest
include(FetchContent)

# Externally provided libraries
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    GIT_TAG v1.14.0)

FetchContent_Declare(benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    GIT_TAG v1.8.3)

FetchContent_MakeAvailable(googletest)

# For benchmark we want to set the BENCMARK_ENABLE_TESTING to OFF therefore we cannot use FetchContent_MakeAvailable()
# Check if population has already been performed
FetchContent_GetProperties(benchmark)
if(NOT benchmark_POPULATED)
  # Fetch the content using previously declared details
  FetchContent_Populate(benchmark)

  # Set custom variables, policies, etc.
  set(BENCHMARK_ENABLE_TESTING OFF)
  set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "Enable testing of the benchmark library." FORCE)

  # Bring the populated content into the build
  add_subdirectory(${benchmark_SOURCE_DIR} ${benchmark_BINARY_DIR})
endif()

# Make a test executable for every plugin
foreach(DIR IN LISTS SUB_DIRS_RELATIVE)

    # get all test cpp and header files
    file(GLOB_RECURSE TEST_SOURCES_${DIR} CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test/${DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/test/${DIR}/*.h)
    file(GLOB_RECURSE SOURCES_${DIR} CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/${DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/source/${DIR}/*.h)

    if (NOT TEST_SOURCES_${DIR})
        message(STATUS "No test files found for ${DIR}")
        continue()
    else ()
        # Setup the test executable
        add_executable(Test_${DIR} ${TEST_SOURCES_${DIR}} ${SOURCES_${DIR}})
    endif ()

    # Add include directories for all folders in test
    file(GLOB_RECURSE TEST_DIRS_${DIR} LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR}/test/${DIR}/*)
    list(APPEND TEST_DIRS_${DIR} ${CMAKE_CURRENT_LIST_DIR}/test/${DIR})
    file(GLOB_RECURSE SOURCE_DIRS_${DIR} LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR}/source/${DIR}/*)
    list(APPEND SOURCE_DIRS_${DIR} ${CMAKE_CURRENT_LIST_DIR}/source/${DIR})

    message(STATUS "Found test files: ${TEST_DIRS_${DIR}}")
    message(STATUS "Found test source files: ${SOURCE_DIRS_${DIR}}")
    message(STATUS "Debug: ${CMAKE_CURRENT_SOURCE_DIR}/source/${DIR}")

    foreach (TEST_DIR IN LISTS TEST_DIRS_${DIR})
        if (IS_DIRECTORY ${TEST_DIR})
            message(STATUS "Including directory test ${TEST_DIR}")
            target_include_directories(Test_${DIR} PRIVATE ${TEST_DIR})
        endif ()
    endforeach ()

    foreach (SOURCE_DIR IN LISTS SOURCE_DIRS_${DIR})
        if (IS_DIRECTORY ${SOURCE_DIR})
            target_include_directories(Test_${DIR} PRIVATE ${SOURCE_DIR})
            message(STATUS "Including directory test ${SOURCE_DIR}")
        endif ()
    endforeach ()

    # Link the test executable against gtest_main and google benchmark
    target_link_libraries(Test_${DIR} gtest_main benchmark::benchmark jack lo)

    # include Loads and runs CMake code from the file given. Loads and runs CMake code from the file given.
    include(GoogleTest)
    # gtest_discover_tests will register a CTest test for each gtest and run them all in parallel with the rest of the Test.
    gtest_discover_tests(Test_${DIR} DISCOVERY_TIMEOUT 90)    
endforeach()