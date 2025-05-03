cmake_minimum_required(VERSION 3.10)

project(HaffmanCodeProject)

# option(BUILD_TESTS "Build the tests" ON)

enable_testing()

include_directories(include)

set(COMMON_TEST_SOURCES
    test/catch_amalgamated.cpp
    src/Haffman.cpp
)

add_executable(BitWriterTests
    test/test_bitwriter.cpp
    ${COMMON_TEST_SOURCES})

# add_executable(CharCountingTests
#     test/test_charCounting.cpp
#     ${COMMON_TEST_SOURCES}) 

add_executable(HaffmanTree
    test/test_HaffmanTree.cpp
    ${COMMON_TEST_SOURCES}) 

include(CTest)
add_test(NAME BitWriterTests COMMAND BitWriterTests)
# add_test(NAME CharCountingTests COMMAND CharCountingTests)
add_test(NAME HaffmanTree COMMAND HaffmanTree)


add_executable(a.out
    src/visual_worl.cpp
    src/Haffman.cpp)
