#pragma once
#include <initializer_list>   // para los for (Op op : {...})
// Tarea 4: cubo semantico de BabyDuck (Tarea 6: se agrega STRING para los letreros)
// Responde: si opero un tipo IZQ con un tipo DER usando OP, que tipo resulta?

// Tipos del lenguaje. BOOL no se puede declarar, pero es lo que dan > < !=
// (lo usan if y while). VOID es para funciones. STRING = letreros de print
// (no se opera con ellos). ERROR = combinacion invalida.
enum class Tipo { INT, FLOAT, BOOL, VOID, STRING, ERROR };

// Operadores de BabyDuck
enum class Op { SUMA, RESTA, MULT, DIV, MAYOR, MENOR, DIF, ASIG };

class CuboSemantico {
public:
    CuboSemantico() {
        // 1) Todo empieza como ERROR: lo que no se ponga abajo es invalido
        for (int i = 0; i < NUM_TIPOS; i++)
            for (int j = 0; j < NUM_TIPOS; j++)
                for (int k = 0; k < NUM_OPS; k++)
                    cubo[i][j][k] = Tipo::ERROR;

        // 2) Aritmeticos (+ - *): int con int da int; si hay un float da float
        for (Op op : {Op::SUMA, Op::RESTA, Op::MULT}) {
            poner(Tipo::INT,   Tipo::INT,   op, Tipo::INT);
            poner(Tipo::INT,   Tipo::FLOAT, op, Tipo::FLOAT);
            poner(Tipo::FLOAT, Tipo::INT,   op, Tipo::FLOAT);
            poner(Tipo::FLOAT, Tipo::FLOAT, op, Tipo::FLOAT);
        }

        // 3) Division: siempre float (5 / 2 = 2.5, no se pierde el decimal)
        poner(Tipo::INT,   Tipo::INT,   Op::DIV, Tipo::FLOAT);
        poner(Tipo::INT,   Tipo::FLOAT, Op::DIV, Tipo::FLOAT);
        poner(Tipo::FLOAT, Tipo::INT,   Op::DIV, Tipo::FLOAT);
        poner(Tipo::FLOAT, Tipo::FLOAT, Op::DIV, Tipo::FLOAT);

        // 4) Relacionales (> < !=): cualquier mezcla de int/float da BOOL
        for (Op op : {Op::MAYOR, Op::MENOR, Op::DIF}) {
            poner(Tipo::INT,   Tipo::INT,   op, Tipo::BOOL);
            poner(Tipo::INT,   Tipo::FLOAT, op, Tipo::BOOL);
            poner(Tipo::FLOAT, Tipo::INT,   op, Tipo::BOOL);
            poner(Tipo::FLOAT, Tipo::FLOAT, op, Tipo::BOOL);
        }

        // 5) Asignacion (variable = valor)
        poner(Tipo::INT,   Tipo::INT,   Op::ASIG, Tipo::INT);
        poner(Tipo::FLOAT, Tipo::FLOAT, Op::ASIG, Tipo::FLOAT);
        poner(Tipo::FLOAT, Tipo::INT,   Op::ASIG, Tipo::FLOAT);  // se promueve
        // int = float queda como ERROR (se perderia el decimal)
    }

    // Consulta: O(1), es solo leer una casilla del arreglo
    Tipo resultado(Tipo izq, Tipo der, Op op) const {
        return cubo[(int)izq][(int)der][(int)op];
    }

private:
    static const int NUM_TIPOS = 6;
    static const int NUM_OPS = 8;
    Tipo cubo[NUM_TIPOS][NUM_TIPOS][NUM_OPS];   // [izq][der][operador]

    void poner(Tipo izq, Tipo der, Op op, Tipo res) {
        cubo[(int)izq][(int)der][(int)op] = res;
    }
};

// Para imprimir un tipo (mensajes y tablas)
inline const char* nombreTipo(Tipo t) {
    switch (t) {
        case Tipo::INT:   return "int";
        case Tipo::FLOAT: return "float";
        case Tipo::BOOL:  return "bool";
        case Tipo::VOID:  return "void";
        case Tipo::STRING: return "string";
        default:          return "error";
    }
}
