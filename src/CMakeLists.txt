cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_SOURCE_PATH})
add_executable(tdy tidy.cpp html_token.cpp html_stream_parser.cpp tidy_parser.cpp)
# target_link_libraries(tidy)
