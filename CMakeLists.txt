project(easyhttp-cpp)

cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_subdirectory(test)

add_test(Tests TestAll)

