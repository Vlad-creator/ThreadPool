cmake_minimum_required(VERSION 3.14)

project(ThreadPool)

set(CMAKE_CXX_STANDARD 17)

# Setup testing
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        703bd9caab50b139428cea1aaff9974ebee5742e
)

enable_testing()

FetchContent_MakeAvailable(googletest)

set (INCLUDES includes)
set (SOURCE source)
set (TEST_DIR tests)

set (HEADERS ${INCLUDES}/thread_pool.h)
set (SRCs ${SOURCE}/thread_pool.cpp)
set (TESTs ${TEST_DIR}/gtests.h)

#main
set (CMAKE_CXX_FLAGS "-pthread")
add_executable (main main.cpp ${HEADERS} ${SRCs})

# test
add_executable(RunUnitTests main.cpp ${SRCs} ${HEADERS} ${TESTs})
target_compile_definitions(RunUnitTests PRIVATE TEST)

include(GoogleTest)
gtest_discover_tests(RunUnitTests)

target_include_directories(RunUnitTests PRIVATE ${TEST_DIR})
target_include_directories(RunUnitTests PRIVATE ${INCLUDES})
target_include_directories(RunUnitTests PRIVATE ${SOURCE})

target_link_libraries(RunUnitTests gtest gtest_main)
