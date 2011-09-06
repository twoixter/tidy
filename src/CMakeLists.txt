find_package(libXml2)

include_directories(${CMAKE_SOURCE_PATH} ${LIBXML2_INCLUDE_DIR})
add_executable(tidy tidy.cpp html_parser.cpp)
target_link_libraries(tidy ${LIBXML2_LIBRARIES})
