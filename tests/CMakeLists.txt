cmake_minimum_required(VERSION 4.0.0)

add_subdirectory(googletest)
include(GoogleTest)

# set (CMAKE_CURRENT_BINARY_DIR bin)

if (CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)

    add_compile_options(-fsanitize=undefined)
    add_link_options(-fsanitize=undefined)
    message("-- Debug Mode: Enabling address and undefined behavior sanitizers")
endif()


macro (DS_add_test testname src)
    add_executable(${testname} ${src})
    target_link_libraries(${testname} PRIVATE GTest::gtest)
    gtest_discover_tests(${testname})
endmacro()

macro(add_typed_test testname src)
    add_executable(${testname} ${src})
    target_link_libraries(${testname} PRIVATE GTest::gtest)
    gtest_discover_tests(${testname} TEST_LIST .TYPED)
endmacro()


add_executable(cuckoo-hash cuckoo-test.cpp)
add_executable(launching-threadpool launching-threadpool-test.cpp)

DS_add_test(unrolled-ll ULL-test.cpp)
DS_add_test(splay-tree splay-tree-test.cpp)
DS_add_test(scapegoat-tree scapegoat-test.cpp)
DS_add_test(median median-test.cpp)
# DS_add_test(kd-tree kdtree-test.cpp)
add_typed_test(tree tree-test.cpp)
add_typed_test(heap heap-test.cpp)



# add_executable(scapegoat-tree scapegoat-test.cpp)
# add_executable(splay-tree splay-tree-test.cpp)
# add_executable(tree tree-test.cpp)
# add_executable(unrolled-ll ULL-test.cpp)
# add_executable(HeapTest heap-test.cpp)
# add_executable(median median-test.cpp)
add_executable(kd-tree kdtree-test.cpp)

# Link to google test if necessary
# target_link_libraries(unrolled-ll gtest) 
# target_link_libraries(splay-tree gtest)
# target_link_libraries(HeapTest PRIVATE GTest::gtest)
# target_link_libraries(tree gtest)
# target_link_libraries(median gtest)
# target_link_libraries(scapegoat-tree gtest)
target_link_libraries(kd-tree gtest gmock)


