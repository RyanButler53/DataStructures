cmake_minimum_required(VERSION 4.0.0)

add_subdirectory(googletest)
include(GoogleTest)

add_compile_options($<$<CONFIG:Debug>:-fsanitize=address>)
add_link_options($<$<CONFIG:Debug>:-fsanitize=address>)

add_compile_options($<$<CONFIG:Debug>:-fsanitize=undefined>)
add_link_options($<$<CONFIG:Debug>:-fsanitize=undefined>)


macro (DS_add_test testname src)
    add_executable("${testname}Test" ${src})
    target_link_libraries("${testname}Test" PRIVATE GTest::gtest GTest::gtest_main)
    gtest_discover_tests("${testname}Test")
endmacro()

macro(add_typed_test testname src)
    add_executable("${testname}Test" ${src})
    target_link_libraries("${testname}Test" PRIVATE GTest::gtest GTest::gtest_main)
    gtest_discover_tests("${testname}Test" TEST_LIST .TYPED)
endmacro()

# Non gtests tests
add_executable(cuckoo-hash cuckoo-test.cpp)
add_executable(launching-threadpool launching-threadpool-test.cpp)

# KD Tree test requires gmock
add_executable(kdtreeTest kdtree-test.cpp)
target_link_libraries(kdtreeTest GTest::gtest GTest::gmock GTest::gtest_main)
gtest_discover_tests(kdtreeTest)


DS_add_test(unrolled-ll ULL-test.cpp)
DS_add_test(splay-tree splay-tree-test.cpp)
DS_add_test(scapegoat-tree scapegoat-test.cpp)
DS_add_test(median median-test.cpp)
DS_add_test(interval-tree intervalTree-test.cpp)
add_typed_test(tree tree-test.cpp)
add_typed_test(heap heap-test.cpp)
