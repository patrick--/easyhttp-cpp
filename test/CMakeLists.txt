include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY "https://github.com/catchorg/Catch2"
  )

#FetchContent_MakeAvailable(Catch2)
FetchContent_GetProperties(Catch2)
if(NOT googletest_POPULATED)
  FetchContent_Populate(Catch2)
  add_subdirectory(${Catch2_SOURCE_DIR} ${Catch2_BINARY_DIR})
endif()

find_package(CURL REQUIRED)
find_package(ZLIB REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/include  ${CURL_INCLUDE_DIRS})
message("curl directories: " ${CURL_INCLUDE_DIRS})

add_executable(TestAll
  test.cpp
  )

target_link_libraries(TestAll Catch2::Catch2 ZLIB::ZLIB ${CURL_LIBRARIES})