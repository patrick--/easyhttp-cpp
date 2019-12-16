project(easyhttp-cpp)

cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_subdirectory(test)

add_test(Tests TestAll)

