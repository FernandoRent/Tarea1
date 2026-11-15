# Tarea 5: BabyDuck con pilas y fila de cuadruplos (expresiones y estatutos lineales)
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

add_executable(babyduck5
    ${BISON_bd_parser_OUTPUTS}
    ${FLEX_bd_lexer_OUTPUTS}
)

target_include_directories(babyduck5 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Para usar Stack.hpp y Queue.hpp de la Tarea 1 (include/structures)
target_link_libraries(babyduck5 PRIVATE structures)

if(MSVC)
    target_compile_options(babyduck5 PRIVATE /W4)
else()
    target_compile_options(babyduck5 PRIVATE -Wall -Wextra -pedantic)
endif()

# ---------------- Pruebas con CTest ----------------
# Cada .txt de tests/validos debe ser ACEPTADO (el programa regresa 0).
file(GLOB CASOS_VALIDOS ${CMAKE_CURRENT_SOURCE_DIR}/tests/validos/*.txt)
foreach(caso ${CASOS_VALIDOS})
    get_filename_component(nombre ${caso} NAME_WE)
    add_test(NAME babyduck5_${nombre} COMMAND babyduck5 ${caso})
endforeach()

# Cada .txt de tests/errores debe ser RECHAZADO (el programa regresa 1).
file(GLOB CASOS_ERRORES ${CMAKE_CURRENT_SOURCE_DIR}/tests/errores/*.txt)
foreach(caso ${CASOS_ERRORES})
    get_filename_component(nombre ${caso} NAME_WE)
    add_test(NAME babyduck5_${nombre} COMMAND babyduck5 ${caso})
    set_tests_properties(babyduck5_${nombre} PROPERTIES WILL_FAIL TRUE)
endforeach()
