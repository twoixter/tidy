cmake_minimum_required(VERSION 2.8)
project(tidy)

include_directories(${CMAKE_SOURCE_PATH})
find_package(libXml2)
add_executable(tidy tidy.cpp)
target_link_libraries(tidy ${LIBXML2_LIBRARIES})
