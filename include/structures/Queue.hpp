/**
 *  @file Queue.hpp
 *  @brief Implementacion de una fila FIFO
 */ 
#pragma once 
#include "structures/Exceptions.hpp"
#include <deque>
#include <cstddef>
#include <utility>
#include <ostream>
namespace structures{
/**
 * @brief Fila FIFO generica: el primero en entrar es el primero en salir.
 *
 * Usa un std::deque por dentro: se agrega al final y se saca del frente.
 * @tparam T Tipo de los elementos.
 */
template <typename T>
class Queue{
    private:
     std::deque<T> data_;
    public:
        /** @brief Iterador de solo lectura, del frente al final. */
        using const_iterator = typename std::deque<T>::const_iterator;

        /**
         * @brief Indica si la fila esta vacia.
         * @return true si no hay elementos.
         * @note Complejidad: O(1).
         */
        bool isEmpty() const{
            return data_.empty();
        }

        /**
         * @brief Numero de elementos en la fila.
         * @note Complejidad: O(1).
         */
        std::size_t size() const{
            return data_.size();
        }

        /**
         * @brief Elimina todos los elementos.
         * @note Complejidad: O(n).
         */
        void clear(){
             data_.clear();
        }

        /**
         * @brief Agrega una copia de valor al final.
         * @param valor Elemento a copiar.
         * @note Complejidad: O(1).
         */
        void enqueue(const T&  valor){
            data_.push_back(valor);
        }

        /**
         * @brief Mueve valor al final, sin copiarlo.
         * @param valor Elemento a mover.
         * @note Complejidad: O(1).
         */
        void enqueue(T&& valor){
            data_.push_back(std::move(valor));
        }

        /**
         * @brief Quita el elemento del frente y lo regresa.
         * @return El elemento que estaba al frente.
         * @throws EmptyQueueException si la fila esta vacia.
         * @note Complejidad: O(1).
         */
        T dequeue() {
            if (data_.empty()){
                throw EmptyQueueException();
            }
            T primero = std::move(data_.front());
            data_.pop_front();
            return primero;
        }

        /**
         * @brief Acceso al frente sin quitarlo; permite modificarlo.
         * @return Referencia al primer elemento.
         * @throws EmptyQueueException si la fila esta vacia.
         * @note Complejidad: O(1).
         */
        T& front() {
            if (data_.empty()) {
                throw EmptyQueueException();
            }
            return data_.front();
        }

        /**
         * @brief Acceso de solo lectura al frente.
         * @return Referencia constante al primer elemento.
         * @throws EmptyQueueException si la fila esta vacia.
         * @note Complejidad: O(1).
         */
        const T& front() const{
            if (data_.empty()) {
                throw EmptyQueueException();
            }
            return data_.front();
        }

        /**
         * @brief Acceso al final sin quitarlo; permite modificarlo.
         * @return Referencia al ultimo elemento.
         * @throws EmptyQueueException si la fila esta vacia.
         * @note Complejidad: O(1).
         */
        T& back() {
            if (data_.empty()) {
                throw EmptyQueueException();
            }
            return data_.back();
        }

        /**
         * @brief Acceso de solo lectura al final.
         * @return Referencia constante al ultimo elemento.
         * @throws EmptyQueueException si la fila esta vacia.
         * @note Complejidad: O(1).
         */
        const T& back() const{
            if (data_.empty()) {
                throw EmptyQueueException();
            }
            return data_.back();
        }

        /**
         * @brief Inicio del recorrido (el frente).
         * @note Complejidad: O(1).
         */
        const_iterator begin() const{
            return data_.cbegin();
        }

        /**
         * @brief Fin del recorrido (despues del ultimo).
         * @note Complejidad: O(1).
         */
        const_iterator end() const{
            return data_.cend();
        }

};

/**
 * @brief Imprime la fila del frente al final como [a, b, c].
 * @tparam T Tipo de los elementos.
 * @param os Stream de salida.
 * @param pila Fila a imprimir.
 * @return El mismo stream, para encadenar <<.
 * @note Complejidad: O(n).
 */
template <typename T>
std::ostream& operator<<(std::ostream& os, const Queue<T>& pila) {
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

