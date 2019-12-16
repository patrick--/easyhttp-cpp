include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY "https://github.com/catchorg/Catch2"
  )

FetchContent_MakeAvailable(Catch2)

add_executable(TestAll
  test.cpp
  )

target_link_libraries(TestAll Catch2::Catch2)