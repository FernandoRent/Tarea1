#pragma once
// Tarea 5: cuadruplos, las 3 pilas y la fila de BabyDuck
#include <iomanip>      // std::setw para alinear la tabla
#include <iostream>
#include <string>       // std::string, std::to_string
#include "structures/Stack.hpp"   // pila propia (Tarea 1)
#include "structures/Queue.hpp"   // fila propia (Tarea 1)
#include "Semantica.hpp"          // enum Tipo

// Un cuadruplo: (operador, operando izquierdo, operando derecho, resultado)
// Ejemplo: a + b  ->  (+, a, b, t1)
struct Cuadruplo {
    std::string op;
    std::string izq;
    std::string der;
    std::string res;
};

inline std::ostream& operator<<(std::ostream& os, const Cuadruplo& q) {
    os << "(" << q.op << ", " << q.izq << ", " << q.der << ", " << q.res << ")";
    return os;
}

// Junta las 3 pilas y la fila que usan los puntos neuralgicos del parser
class GeneradorCuadruplos {
public:
    structures::Stack<std::string> pOperadores;  // "+", "*", ">", ... y "(" como fondo falso
    structures::Stack<std::string> pOperandos;   // nombres: a, b, 5, t1 ...
    structures::Stack<Tipo>        pTipos;       // tipo de cada operando (va en paralelo)
    structures::Queue<Cuadruplo>   fila;         // cuadruplos en el orden en que se generan

    // Pide el siguiente temporal: t1, t2, t3, ...
    std::string nuevoTemporal() {
        return "t" + std::to_string(++contTemporales);
    }

    // Formar un cuadruplo nuevo al final de la fila
    void generar(const std::string& op, const std::string& izq,
                 const std::string& der, const std::string& res) {
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
                      << "| " << std::setw(8) << q.izq << "| " << std::setw(8) << q.der
                      << "| " << q.res << "\n";
        }
    }

private:
    int contTemporales = 0;
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
