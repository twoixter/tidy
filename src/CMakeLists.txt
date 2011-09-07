include_directories(${CMAKE_SOURCE_PATH})
add_executable(tidy tidy.cpp html_token.cpp html_stream_parser.cpp)
target_link_libraries(tidy)
