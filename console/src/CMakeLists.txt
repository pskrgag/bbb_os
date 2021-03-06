set(CMAKE_CXX_STANDARD 14)

set(INCLUDE_DIR ../include)

add_executable(${PROJECT_NAME} console.cpp
                               mainwindow.cpp ${INCLUDE_DIR}/mainwindow.h
                               olafstatuscheker.cpp ${INCLUDE_DIR}/olafstatuscheker.h
                               olafclient.cpp ${INCLUDE_DIR}/olafclient.h
                               )

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

IF(CMAKE_BUILD_TYPE MATCHES Debug)
        target_link_directories(${PROJECT_NAME} PRIVATE ../../../build/libs)
        target_include_directories(${PROJECT_NAME} PUBLIC ../../include ../../libs/libolaf/include/)
ENDIF()

find_package(Qt5 COMPONENTS Gui Core Widgets Network REQUIRED)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Network -lolaf)
