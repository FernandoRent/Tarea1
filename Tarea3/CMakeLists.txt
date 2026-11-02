# Tarea 3: scanner (Flex) y parser (Bison) de BabyDuck
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

# Generar el parser con Bison (parser.cpp + parser.hpp con los tokens)
bison_target(
    bd_parser
    babyduck.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.hpp
)

# Generar el scanner con Flex
flex_target(
    bd_lexer
    babyduck.l
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
)

# El scanner necesita parser.hpp, asi que Bison va primero
add_flex_bison_dependency(bd_lexer bd_parser)

add_executable(babyduck
    ${BISON_bd_parser_OUTPUTS}
    ${FLEX_bd_lexer_OUTPUTS}
)

target_include_directories(babyduck PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

if(MSVC)
    target_compile_options(babyduck PRIVATE /W4)
else()
    target_compile_options(babyduck PRIVATE -Wall -Wextra -pedantic)
endif()

# ---------------- Pruebas con CTest ----------------
# Cada .txt de tests/validos debe ser ACEPTADO (el programa regresa 0).
file(GLOB CASOS_VALIDOS ${CMAKE_CURRENT_SOURCE_DIR}/tests/validos/*.txt)
foreach(caso ${CASOS_VALIDOS})
    get_filename_component(nombre ${caso} NAME_WE)
    add_test(NAME babyduck_${nombre} COMMAND babyduck ${caso})
endforeach()

# Cada .txt de tests/errores debe ser RECHAZADO (el programa regresa 1).
file(GLOB CASOS_ERRORES ${CMAKE_CURRENT_SOURCE_DIR}/tests/errores/*.txt)
foreach(caso ${CASOS_ERRORES})
    get_filename_component(nombre ${caso} NAME_WE)
    add_test(NAME babyduck_${nombre} COMMAND babyduck ${caso})
    set_tests_properties(babyduck_${nombre} PROPERTIES WILL_FAIL TRUE)
endforeach()
