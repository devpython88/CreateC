cmake_minimum_required(VERSION 3.5)
project(createc)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/qt5" "/usr/include/x86_64-linux-gnu/qt5")

find_package(Qt5 REQUIRED COMPONENTS Core)
add_executable(${PROJECT_NAME} src/createc.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${Qt5Core_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Qt::Core)