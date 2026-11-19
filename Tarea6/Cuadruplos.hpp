#pragma once
// Tarea 5: cuadruplos, las 3 pilas y la fila de BabyDuck
// Tarea 6: los operandos ahora son direcciones virtuales (numeros)
#include <iomanip>      // std::setw para alinear la tabla
#include <iostream>
#include <string>       // std::string, std::to_string
#include "structures/Stack.hpp"   // pila propia (Tarea 1)
#include "structures/Queue.hpp"   // fila propia (Tarea 1)
#include "Semantica.hpp"          // enum Tipo
#include "Memoria.hpp"            // direcciones virtuales

// Un cuadruplo: (operador, operando izquierdo, operando derecho, resultado)
// Ejemplo: a + b  ->  (+, 1000, 1001, 5000)
// VACIO (-1) = casilla sin usar
const int VACIO = -1;

struct Cuadruplo {
    std::string op;
    int izq;
    int der;
    int res;
};

// Para imprimir una casilla: vacia si no se usa
inline std::string casilla(int x) {
    return x == VACIO ? "" : std::to_string(x);
}

inline std::ostream& operator<<(std::ostream& os, const Cuadruplo& q) {
    os << "(" << q.op << ", " << casilla(q.izq) << ", " << casilla(q.der)
       << ", " << casilla(q.res) << ")";
    return os;
}

// Junta las 3 pilas y la fila que usan los puntos neuralgicos del parser
class GeneradorCuadruplos {
public:
    structures::Stack<std::string> pOperadores;  // "+", "*", ">", ... y "(" como fondo falso
    structures::Stack<int>         pOperandos;   // direcciones: 1000, 8000, 5000 ...
    structures::Stack<Tipo>        pTipos;       // tipo de cada operando (va en paralelo)
    structures::Stack<int>         pSaltos;      // Tarea 6: cuadruplos con salto pendiente y regresos de while
    structures::Queue<Cuadruplo>   fila;         // cuadruplos en el orden en que se generan
    MemoriaVirtual                 mem;          // reparte las direcciones

    // Pide el siguiente temporal del tipo que dio el cubo (int 5000, float 6000, bool 7000)
    int nuevoTemporal(Tipo t) {
        return mem.pedir(Segmento::TEMPORAL, t);
    }

    // Numero que tendra el siguiente cuadruplo (se numeran desde 1)
    int siguiente() const {
        return (int)fila.size() + 1;
    }

    // Rellenar el salto pendiente del cuadruplo numero 'cuad' para que vaya a 'destino'
    void rellenar(int cuad, int destino) {
        fila.at(cuad - 1).res = destino;
    }

    // Formar un cuadruplo nuevo al final de la fila
    void generar(const std::string& op, int izq, int der, int res) {
        fila.enqueue(Cuadruplo{op, izq, der, res});
    }

    // Desplegar la fila como tabla, del primero al ultimo
    void imprimir() const {
        std::cout << "\nFila de cuadruplos (" << fila.size() << ")\n";
        std::cout << std::left
                  << std::setw(4) << "#"   << "| " << std::setw(8) << "op"
                  << "| " << std::setw(8) << "izq" << "| " << std::setw(8) << "der"
                  << "| " << "res" << "\n";
        std::cout << "----+---------+---------+---------+-------\n";
        int i = 1;
        for (const Cuadruplo& q : fila) {
            std::cout << std::setw(4) << i++ << "| " << std::setw(8) << q.op
                      << "| " << std::setw(8) << casilla(q.izq) << "| " << std::setw(8)
                      << casilla(q.der) << "| " << casilla(q.res) << "\n";
        }
    }
};

// Convierte el operador en texto ("+", ">", ...) al Op del cubo semantico
inline Op textoAOp(const std::string& op) {
    if (op == "+")  return Op::SUMA;
    if (op == "-")  return Op::RESTA;
    if (op == "*")  return Op::MULT;
    if (op == "/")  return Op::DIV;
    if (op == ">")  return Op::MAYOR;
    if (op == "<")  return Op::MENOR;
    if (op == "!=") return Op::DIF;
    return Op::ASIG;   // "="
}
