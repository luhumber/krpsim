cmake_minimum_required(VERSION 3.16)
project(Krpsim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Concurrent)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/gui/MainWindow.cpp
    src/gui/MainWindow.h
    src/gui/MainWindow.ui
    src/gui/tree_graphics_view/TreeGraphicsView.cpp
    src/gui/tree_graphics_view/TreeGraphicsView.h
    src/core/parser/Parser.cpp
    src/core/parser/regex/RegexDefs.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src
    src/gui
    src/core
    src/core/parser
    src/core/parser/regex
    src/core/simulator
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Concurrent)

