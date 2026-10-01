/**
 *  @file Stack.hpp
 *  @brief Implementacion de una pila LIFO
 */ 
#pragma once 
#include "structures/Exceptions.hpp"
#include <vector>
#include <cstddef>
#include <utility>
#include <ostream>
namespace structures{
/**
 * @brief Pila LIFO generica: el ultimo en entrar es el primero en salir.
 *
 * Usa un std::vector por dentro; el tope es el final del vector.
 * @tparam T Tipo de los elementos.
 */
template <typename T>
class Stack{
    private:
     std::vector<T> Stack_;
    public:
        /** @brief Iterador de solo lectura, del tope al fondo. */
        using const_iterator = typename std::vector<T>::const_reverse_iterator;

        /**
         * @brief Indica si la pila esta vacia.
         * @return true si no hay elementos.
         * @note Complejidad: O(1).
         */
        bool isEmpty() const{
            return Stack_.empty();
        }

        /**
         * @brief Numero de elementos en la pila.
         * @note Complejidad: O(1).
         */
        std::size_t size() const{
            return Stack_.size();
        }

        /**
         * @brief Elimina todos los elementos.
         * @note Complejidad: O(n).
         */
        void clear(){
             Stack_.clear();
        }

        /**
         * @brief Agrega una copia de valor en el tope.
         * @param valor Elemento a copiar.
         * @note Complejidad: O(1) amortizado.
         */
        void push(const T&  valor){
            Stack_.push_back(valor);
        }

        /**
         * @brief Mueve valor al tope, sin copiarlo.
         * @param valor Elemento a mover.
         * @note Complejidad: O(1) amortizado.
         */
        void push(T&& valor){
            Stack_.push_back(std::move(valor));
        }

        /**
         * @brief Quita el tope y lo regresa.
         * @return El elemento que estaba en el tope.
         * @throws EmptyStackException si la pila esta vacia.
         * @note Complejidad: O(1).
         */
        T pop() {
            if (Stack_.empty()){
                throw EmptyStackException();
            }
            T tope = std::move(Stack_.back());
            Stack_.pop_back();
            return tope;
        }

        /**
         * @brief Acceso al tope sin quitarlo; permite modificarlo.
         * @return Referencia al tope.
         * @throws EmptyStackException si la pila esta vacia.
         * @note Complejidad: O(1).
         */
        T& peek() {
            if (Stack_.empty()) {
                throw EmptyStackException();
            }
            return Stack_.back();
        }

        /**
         * @brief Acceso de solo lectura al tope.
         * @return Referencia constante al tope.
         * @throws EmptyStackException si la pila esta vacia.
         * @note Complejidad: O(1).
         */
        const T& peek() const{
            if (Stack_.empty()) {
                throw EmptyStackException();
            }
            return Stack_.back();
        }

        /**
         * @brief Inicio del recorrido (el tope).
         * @note Complejidad: O(1).
         */
        const_iterator begin() const{
            return Stack_.crbegin();
        }

        /**
         * @brief Fin del recorrido (despues del fondo).
         * @note Complejidad: O(1).
         */
        const_iterator end() const{
            return Stack_.crend();
        }

};

/**
 * @brief Imprime la pila del tope al fondo como [a, b, c].
 * @tparam T Tipo de los elementos.
 * @param os Stream de salida.
 * @param pila Pila a imprimir.
 * @return El mismo stream, para encadenar <<.
 * @note Complejidad: O(n).
 */
template <typename T>
std::ostream& operator<<(std::ostream& os, const Stack<T>& pila) {
    os << "[";
    bool primero = true; 
    for (const auto& elemento : pila) {
        if (!primero){
            os << ", ";
        }
        os << elemento;
        primero = false;
    }
    os << "]";
    return os;
}



} //namespace structures 

