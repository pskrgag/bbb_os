cmake_minimum_required(VERSION 3.16)

project(console)

set(INSTALL_DIR ~/console)

add_subdirectory(src)

file(GLOB PNGS "${CMAKE_CURRENT_SOURCE_DIR}/docs/*")
file(COPY ${PNGS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/src)
