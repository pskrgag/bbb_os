cmake_minimum_required(VERSION 3.16)
project(console)

set(CMAKE_CXX_STANDARD 14)
link_directories($ENV{LD_LIBRARY_PATH})

add_executable(${PROJECT_NAME} console.cpp mainwindow.cpp mainwindow.ui mainwindow.hpp olafclient.h olafclient.cpp)
set_target_properties(console PROPERTIES AUTOMOC ON AUTOUIC ON)

find_package(Qt5 COMPONENTS Gui Core Widgets REQUIRED)
target_link_libraries(console Qt5::Core Qt5::Widgets Qt5::Gui -lolaf)
