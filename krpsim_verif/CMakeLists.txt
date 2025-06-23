add_executable(krpsim_verif
    main.cpp
    ../src/core/parser/Parser.cpp
    ../src/core/parser/regex/RegexDefs.cpp
    ../src/core/simulator/Stock.cpp
    ../src/core/simulator/TraceWriter.cpp
)

target_include_directories(krpsim_verif PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/core/parser
    ${CMAKE_SOURCE_DIR}/src/core/parser/regex
    ${CMAKE_SOURCE_DIR}/src/core/simulator
    ${CMAKE_SOURCE_DIR}/src/core/beam_search/
    ${CMAKE_SOURCE_DIR}/src/core/beam_search/beam_node
)

target_link_libraries(krpsim_verif PRIVATE Qt6::Core)
