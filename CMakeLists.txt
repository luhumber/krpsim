cmake_minimum_required(VERSION 3.16)
project(Krpsim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Ajout des flags de débogage
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

set(CMAKE_PREFIX_PATH "/home/luhumber/goinfre/Qt/6.9.1/gcc_64" CACHE PATH "Qt installation prefix")

find_package(Qt6 REQUIRED COMPONENTS Widgets Concurrent Charts)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/gui/MainWindow.cpp
    src/gui/MainWindow.ui
    src/gui/tree_graphics_view/TreeGraphicsView.cpp
    src/gui/beam_table_view/BeamTableView.cpp
    src/core/parser/Parser.cpp
    src/core/parser/regex/RegexDefs.cpp
    src/core/beam_search/BeamSearch.cpp
    src/core/beam_search/beam_search_worker/BeamSearchWorker.cpp
    src/core/beam_search/beam_node/BeamNode.cpp
    src/core/simulator/Stock.cpp
    src/core/simulator/TraceWriter.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src
    src/gui
    src/gui/tree_graphics_view
    src/gui/beam_table_view
    src/core
    src/core/parser
    src/core/parser/regex
    src/core/simulator
    src/core/beam_search
    src/core/beam_search/beam_search_worker
    src/core/beam_search/beam_node
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Concurrent Qt6::Charts)

add_subdirectory(krpsim_verif)