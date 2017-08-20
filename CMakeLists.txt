cmake_minimum_required(VERSION 3.5)
project(giygas CXX)

enable_testing()
find_package(GTest REQUIRED)

file(GLOB_RECURSE GIYGAS_SOURCE_FILES "./src/*" "./include/*")
file(GLOB_RECURSE GIYGAS_TEST_SOURCE_FILES "./test/*")

add_library(giygas SHARED ${GIYGAS_SOURCE_FILES})
set_target_properties(giygas PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)

target_include_directories(giygas PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_executable(giygas_test ${GIYGAS_TEST_SOURCE_FILES})
target_link_libraries(giygas_test GTest::GTest GTest::Main)
add_test(GiygasTests giygas_test)

