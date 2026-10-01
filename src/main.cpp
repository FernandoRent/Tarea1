/**
 *  @file main.cpp
 *  @brief Programa demo: manipula Stack, Queue y Dictionary e imprime cada paso.
 *
 *  Los ejemplos imitan el uso que tendran en el compilador del curso:
 *  pila de operandos, fila de cuadruplos y tabla de simbolos.
 */
#include "structures/Stack.hpp"
#include "structures/Queue.hpp"
#include "structures/Dictionary.hpp"
#include <iostream>
#include <sstream>
#include <string>

using structures::Stack;
using structures::Queue;
using structures::Dictionary;

/** @brief Imprime un encabezado de seccion. */
void titulo(const std::string& texto){
    std::cout << "\n==================================================\n";
    std::cout << " " << texto << "\n";
    std::cout << "==================================================\n";
}

/**
 * @brief Evalua una expresion postfija (ej. "3 4 + 2 *") con una pila de operandos.
 * @param expresion Numeros enteros y operadores + - * separados por espacios.
 * @return El resultado de la expresion.
 */
int evaluarPostfija(const std::string& expresion){
    Stack<int> operandos;
    std::istringstream entrada(expresion);
    std::string token;
    while (entrada >> token){
        if (token == "+" || token == "-" || token == "*"){
            int derecho = operandos.pop();
            int izquierdo = operandos.pop();
            int resultado = 0;
            if (token == "+") resultado = izquierdo + derecho;
            if (token == "-") resultado = izquierdo - derecho;
            if (token == "*") resultado = izquierdo * derecho;
            std::cout << "  " << izquierdo << " " << token << " " << derecho
                      << " = " << resultado << "   pila: " << operandos;
            operandos.push(resultado);
            std::cout << " -> " << operandos << "\n";
        } else{
            operandos.push(std::stoi(token));
            std::cout << "  push(" << token << ")         pila: " << operandos << "\n";
        }
    }
    return operandos.pop();
}

void demoStack(){
    titulo("STACK (pila LIFO)");
    Stack<int> pila;
    std::cout << "Pila nueva: " << pila << "  isEmpty=" << std::boolalpha << pila.isEmpty() << "\n";

    for (int i = 1; i <= 3; ++i){
        pila.push(i * 10);
        std::cout << "push(" << i * 10 << ")  -> " << pila << "  size=" << pila.size() << "\n";
    }
    std::cout << "peek()    -> " << pila.peek() << "  (el tope, sin quitarlo)\n";
    std::cout << "pop()     -> " << pila.pop() << "  pila: " << pila << "\n";

    std::cout << "Recorrido de tope a fondo: ";
    for (const auto& elemento : pila){
        std::cout << elemento << " ";
    }
    std::cout << "\n";

    pila.clear();
    std::cout << "clear()   -> " << pila << "\n";
    try{
        pila.pop();
    } catch (const structures::EmptyStackException& e){
        std::cout << "pop() en pila vacia -> excepcion: " << e.what() << "\n";
    }

    std::cout << "\nUso en el compilador: evaluar \"3 4 + 2 *\" con una pila de operandos\n";
    int resultado = evaluarPostfija("3 4 + 2 *");
    std::cout << "  Resultado: " << resultado << "\n";
}

void demoQueue(){
    titulo("QUEUE (fila FIFO)");
    Queue<std::string> fila;
    std::cout << "Fila nueva: " << fila << "  isEmpty=" << std::boolalpha << fila.isEmpty() << "\n";

    fila.enqueue("(+, a, b, t1)");
    std::cout << "enqueue -> " << fila << "\n";
    fila.enqueue("(*, t1, c, t2)");
    std::cout << "enqueue -> " << fila << "\n";
    fila.enqueue("(=, t2, _, x)");
    std::cout << "enqueue -> " << fila << "\n";

    std::cout << "front() -> " << fila.front() << "  (el primero)\n";
    std::cout << "back()  -> " << fila.back() << "  (el ultimo)\n";

    std::cout << "Generando codigo en orden de llegada (dequeue):\n";
    int numero = 1;
    while (!fila.isEmpty()){
        std::cout << "  " << numero << ". " << fila.dequeue() << "   quedan " << fila.size() << "\n";
        ++numero;
    }

    try{
        fila.front();
    } catch (const structures::EmptyQueueException& e){
        std::cout << "front() en fila vacia -> excepcion: " << e.what() << "\n";
    }
}

void demoDictionary(){
    titulo("DICTIONARY (tabla de simbolos, orden de insercion)");
    Dictionary<std::string, std::string> tabla;
    std::cout << "Tabla nueva: " << tabla << "  isEmpty=" << std::boolalpha << tabla.isEmpty() << "\n";

    tabla.put("x", "int");
    tabla.put("total", "float");
    tabla.put("activo", "bool");
    std::cout << "put x, total, activo -> " << tabla << "\n";

    tabla.put("total", "double");
    std::cout << "put(total, double)   -> " << tabla << "  (actualiza sin mover)\n";

    std::cout << "get(x)               -> " << tabla.get("x") << "\n";
    std::cout << "contains(y)          -> " << tabla.contains("y") << "\n";
    std::cout << "getOr(y, \"?\")        -> " << tabla.getOr("y", "?") << "  (no inserta)\n";

    std::cout << "remove(x)            -> " << tabla.remove("x") << "  tabla: " << tabla << "\n";
    std::cout << "remove(x) otra vez   -> " << tabla.remove("x") << "\n";
    tabla.put("x", "char");
    std::cout << "put(x, char)         -> " << tabla << "  (reinsertada va al final)\n";

    tabla["nombre"] = "string";
    std::cout << "tabla[nombre]=string -> " << tabla << "  (operator[] inserta)\n";

    std::cout << "keys()        -> ";
    for (const auto& llave : tabla.keys()){
        std::cout << llave << " ";
    }
    std::cout << "\nsortedItems() -> ";
    for (const auto& par : tabla.sortedItems()){
        std::cout << par.first << ":" << par.second << " ";
    }
    std::cout << "\n";

    Dictionary<std::string, std::string> copia(tabla);
    copia.put("activo", "int");
    std::cout << "Copia modificada: " << copia << "\n";
    std::cout << "Original intacto: " << tabla << "\n";

    try{
        tabla.get("inexistente");
    } catch (const structures::KeyNotFoundException& e){
        std::cout << "get(inexistente) -> excepcion: " << e.what() << "\n";
    }

    Dictionary<std::string, int> contador;
    for (const std::string palabra : {"a", "b", "a", "c", "a", "b"}){
        contador[palabra] += 1;
    }
    std::cout << "\nContar palabras con operator[] en {a, b, a, c, a, b}: " << contador << "\n";
}

int main(){
    std::cout << "Tarea 1 - Demo de estructuras de datos\n";
    demoStack();
    demoQueue();
    demoDictionary();

    titulo("Cualquier error de las estructuras se atrapa con StructureException");
    try{
        Queue<int> vacia;
        vacia.dequeue();
    } catch (const structures::StructureException& e){
        std::cout << "Atrapada como StructureException: " << e.what() << "\n";
    }
    return 0;
}
