cmake_minimum_required(VERSION 3.16)
project(Krpsim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Concurrent Charts)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/gui/MainWindow.cpp
    src/gui/MainWindow.ui
    src/gui/tree_graphics_view/TreeGraphicsView.cpp
    src/gui/line_chart_widget/LineChartWidget.cpp
    src/gui/bar_chart_widget/BarChartWidget.cpp
    src/gui/beam_table_view/BeamTableView.cpp
    src/core/parser/Parser.cpp
    src/core/parser/regex/RegexDefs.cpp
    src/core/beam_search/BeamSearch.cpp
    src/core/beam_search/beam_node/BeamNode.cpp
    src/core/simulator/Stock.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src
    src/gui
    src/gui/tree_graphics_view
    src/gui/line_chart_widget
    src/gui/bar_chart_widget
    src/gui/beam_table_view
    src/core
    src/core/parser
    src/core/parser/regex
    src/core/simulator
    src/core/beam_search
    src/core/beam_search/beam_node
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Concurrent Qt6::Charts)
