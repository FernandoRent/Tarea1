find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

# Generar parser con Bison
bison_target(
    calc_parser
    calc.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.hpp
)

# Generar lexer con Flex
flex_target(
    calc_lexer
    calc.l
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
)

# Flex depende del parser.hpp generado por Bison
add_flex_bison_dependency(calc_lexer calc_parser)

# Definición del ejecutable
add_executable(calc
    ${BISON_calc_parser_OUTPUTS}
    ${FLEX_calc_lexer_OUTPUTS}
)

# Directorios de inclusión (fuentes para Cuadruplo.hpp y build para parser.hpp)
target_include_directories(calc PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Enlace con la biblioteca de estructuras y configuración de warnings
target_link_libraries(calc PRIVATE structures)

if(MSVC)
    target_compile_options(calc PRIVATE /W4)
else()
    target_compile_options(calc PRIVATE -Wall -Wextra -pedantic)
endif()