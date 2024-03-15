# Wierdly without cache the variable is not set in the global scope
set(TEST_CONFIG_0 "${CMAKE_CURRENT_SOURCE_DIR}/test-configs/test_config_0.yml" CACHE STRING "Path to the test configuration file 0")
add_compile_definitions(TEST_CONFIG_0="${TEST_CONFIG_0}")

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
    file(GLOB_RECURSE TEST_SOURCES_${DIR} CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*.h)
    list(REMOVE_ITEM SOURCES ${CMAKE_SOURCE_DIR}/source/main.cpp)

    if (NOT TEST_SOURCES_${DIR})
        message(STATUS "No test files found for ${DIR}")
        continue()
    else ()
        message(STATUS "Found test files for ${DIR}")
        # Setup the test executable
        add_executable(Test_${DIR} ${TEST_SOURCES_${DIR}} ${SOURCES})
    endif ()

    # Add include directories for all folders in test
    file(GLOB_RECURSE TEST_DIRS_${DIR} LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*)
    list(APPEND TEST_DIRS_${DIR} ${CMAKE_CURRENT_LIST_DIR}/${DIR})

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

    get_target_property(COMPILE_DEF ${PROJECT_NAME} COMPILE_DEFINITIONS)
    target_compile_definitions(Test_${DIR} PRIVATE ${COMPILE_DEF})
    # include Loads and runs CMake code from the file given. Loads and runs CMake code from the file given.
    include(GoogleTest)
    # gtest_discover_tests will register a CTest test for each gtest and run them all in parallel with the rest of the Test.
    gtest_discover_tests(Test_${DIR} DISCOVERY_TIMEOUT 120)    
endforeach()