cmake_minimum_required(VERSION 3.12)
project(console)

set(CMAKE_CXX_STANDARD 14)
add_executable(${PROJECT_NAME} console.cpp)
set_target_properties(console PROPERTIES AUTOMOC ON AUTOUIC ON)
find_package(Qt5 COMPONENTS Gui Core Widgets REQUIRED)
target_link_libraries(console Qt5::Core Qt5::Widgets Qt5::Gui)