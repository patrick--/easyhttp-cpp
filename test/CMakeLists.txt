include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY "https://github.com/catchorg/Catch2"
  )

FetchContent_MakeAvailable(Catch2)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(TestAll
  test.cpp
  )

target_link_libraries(TestAll Catch2::Catch2)