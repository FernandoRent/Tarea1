/**
 *  @file Dictionary.hpp
 *  @brief Implementacion de un diccionario que conserva el orden de insercion
 */
#pragma once
#include "structures/Exceptions.hpp"
#include <list>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iterator>
#include <utility>
#include <cstddef>
#include <ostream>
namespace structures{
/**
 * @brief Diccionario llave -> valor que recuerda el orden de insercion.
 *
 * Usa una std::list para el orden y un std::unordered_map que va de cada
 * llave a su nodo en la lista, para buscar en O(1) promedio.
 * @tparam K Tipo de las llaves (necesita std::hash y ==; sortedItems necesita <).
 * @tparam V Tipo de los valores.
 */
template <typename K, typename V>
class Dictionary{
    public:
        /** @brief Un par llave-valor; la llave es const para no desincronizar el indice. */
        using Item = std::pair<const K, V>;
        /** @brief Lista de pares en orden de insercion. */
        using ItemList = std::list<Item>;
        /** @brief Iterador en orden de insercion; permite cambiar valores, no llaves. */
        using iterator = typename ItemList::iterator;
        /** @brief Iterador de solo lectura en orden de insercion. */
        using const_iterator = typename ItemList::const_iterator;

    private:
        ItemList items_;
        std::unordered_map<K, iterator> index_;

        /**
         * @brief Inserta un par nuevo al final, en la lista y en el indice.
         *
         * Si falla el indice, deshace la insercion en la lista para no
         * dejar las dos estructuras desincronizadas.
         * @return Referencia al valor insertado.
         * @note Complejidad: O(1) promedio.
         */
        V& insertNew(const K& llave, V&& valor){
            items_.emplace_back(llave, std::move(valor));
            try{
                index_.emplace(llave, std::prev(items_.end()));
            } catch (...){
                items_.pop_back();
                throw;
            }
            return items_.back().second;
        }

        /**
         * @brief Vuelve a llenar el indice a partir de items_.
         * @note Complejidad: O(n) promedio.
         */
        void rebuildIndex(){
            index_.clear();
            for (iterator it = items_.begin(); it != items_.end(); ++it){
                index_.emplace(it->first, it);
            }
        }

    public:
        /** @brief Crea un diccionario vacio. */
        Dictionary() = default;

        /**
         * @brief Copia otro diccionario.
         *
         * Copia la lista y reconstruye el indice, porque los iteradores del
         * otro apuntan a SU lista, no a la nueva.
         * @param otro Diccionario a copiar.
         * @note Complejidad: O(n) promedio.
         */
        Dictionary(const Dictionary& otro)
            :items_(otro.items_)
        {
            rebuildIndex();
        }

        /**
         * @brief Mueve otro diccionario (otro queda vacio).
         * @param otro Diccionario a mover.
         * @note Complejidad: O(1).
         */
        Dictionary(Dictionary&& otro) noexcept{
            swap(otro);
        }

        /**
         * @brief Asignacion por copia.
         * @param otro Diccionario a copiar.
         * @return Este diccionario.
         * @note Complejidad: O(n) promedio.
         */
        Dictionary& operator=(const Dictionary& otro){
            if (this != &otro){
                Dictionary copia(otro);
                swap(copia);
            }
            return *this;
        }

        /**
         * @brief Asignacion por movimiento (otro queda vacio).
         * @param otro Diccionario a mover.
         * @return Este diccionario.
         * @note Complejidad: O(n) por liberar el contenido anterior.
         */
        Dictionary& operator=(Dictionary&& otro) noexcept{
            if (this != &otro){
                clear();
                swap(otro);
            }
            return *this;
        }

        /**
         * @brief Intercambia el contenido con otro diccionario.
         * @param otro Diccionario con el que se intercambia.
         * @note Complejidad: O(1).
         */
        void swap(Dictionary& otro) noexcept{
            items_.swap(otro.items_);
            index_.swap(otro.index_);
        }

        /**
         * @brief Indica si el diccionario esta vacio.
         * @return true si no hay elementos.
         * @note Complejidad: O(1).
         */
        bool isEmpty() const{
            return items_.empty();
        }

        /**
         * @brief Numero de pares en el diccionario.
         * @note Complejidad: O(1).
         */
        std::size_t size() const{
            return items_.size();
        }

        /**
         * @brief Elimina todos los pares.
         * @note Complejidad: O(n).
         */
        void clear(){
            items_.clear();
            index_.clear();
        }

        /**
         * @brief Indica si la llave existe.
         * @param llave Llave a buscar.
         * @return true si la llave esta en el diccionario.
         * @note Complejidad: O(1) promedio.
         */
        bool contains(const K& llave) const{
            return index_.find(llave) != index_.end();
        }

        /**
         * @brief Inserta el par, o actualiza el valor si la llave ya existe.
         *
         * Al actualizar, el par conserva su posicion original.
         * @param llave Llave.
         * @param valor Valor a copiar.
         * @note Complejidad: O(1) promedio.
         */
        void put(const K& llave, const V& valor){
            auto encontrado = index_.find(llave);
            if (encontrado != index_.end()){
                encontrado->second->second = valor;
            } else{
                insertNew(llave, V(valor));
            }
        }

        /**
         * @brief Igual que put, pero mueve el valor en vez de copiarlo.
         * @param llave Llave.
         * @param valor Valor a mover.
         * @note Complejidad: O(1) promedio.
         */
        void put(const K& llave, V&& valor){
            auto encontrado = index_.find(llave);
            if (encontrado != index_.end()){
                encontrado->second->second = std::move(valor);
            } else{
                insertNew(llave, std::move(valor));
            }
        }

        /**
         * @brief Acceso al valor de una llave; permite modificarlo.
         * @param llave Llave a buscar.
         * @return Referencia al valor.
         * @throws KeyNotFoundException si la llave no existe.
         * @note Complejidad: O(1) promedio.
         */
        V& get(const K& llave){
            auto encontrado = index_.find(llave);
            if (encontrado == index_.end()){
                throw KeyNotFoundException();
            }
            return encontrado->second->second;
        }

        /**
         * @brief Acceso de solo lectura al valor de una llave.
         * @param llave Llave a buscar.
         * @return Referencia constante al valor.
         * @throws KeyNotFoundException si la llave no existe.
         * @note Complejidad: O(1) promedio.
         */
        const V& get(const K& llave) const{
            auto encontrado = index_.find(llave);
            if (encontrado == index_.end()){
                throw KeyNotFoundException();
            }
            return encontrado->second->second;
        }

        /**
         * @brief Valor de la llave, o porDefecto si no existe. No inserta nada.
         * @param llave Llave a buscar.
         * @param porDefecto Valor a regresar si la llave no existe.
         * @return Una copia del valor encontrado o de porDefecto.
         * @note Complejidad: O(1) promedio.
         */
        V getOr(const K& llave, const V& porDefecto) const{
            auto encontrado = index_.find(llave);
            if (encontrado == index_.end()){
                return porDefecto;
            }
            return encontrado->second->second;
        }

        /**
         * @brief Borra el par de la llave.
         * @param llave Llave a borrar.
         * @return true si existia y se borro; false si no existia.
         * @note Complejidad: O(1) promedio.
         */
        bool remove(const K& llave){
            auto encontrado = index_.find(llave);
            if (encontrado == index_.end()){
                return false;
            }
            items_.erase(encontrado->second);
            index_.erase(encontrado);
            return true;
        }

        /**
         * @brief Como std::map: regresa el valor y, si la llave no existe,
         * la inserta al final con un valor por defecto (V()).
         * @param llave Llave a buscar o insertar.
         * @return Referencia al valor.
         * @note Complejidad: O(1) promedio.
         */
        V& operator[](const K& llave){
            auto encontrado = index_.find(llave);
            if (encontrado != index_.end()){
                return encontrado->second->second;
            }
            return insertNew(llave, V());
        }

        /**
         * @brief Llaves en orden de insercion.
         * @note Complejidad: O(n).
         */
        std::vector<K> keys() const{
            std::vector<K> resultado;
            resultado.reserve(items_.size());
            for (const auto& item : items_){
                resultado.push_back(item.first);
            }
            return resultado;
        }

        /**
         * @brief Valores en orden de insercion.
         * @note Complejidad: O(n).
         */
        std::vector<V> values() const{
            std::vector<V> resultado;
            resultado.reserve(items_.size());
            for (const auto& item : items_){
                resultado.push_back(item.second);
            }
            return resultado;
        }

        /**
         * @brief Copia de los pares en orden de insercion.
         * @note Complejidad: O(n).
         */
        std::vector<std::pair<K, V>> items() const{
            return std::vector<std::pair<K, V>>(items_.begin(), items_.end());
        }

        /**
         * @brief Copia de los pares ordenados por llave (de menor a mayor).
         * @note Complejidad: O(n log n).
         */
        std::vector<std::pair<K, V>> sortedItems() const{
            std::vector<std::pair<K, V>> resultado = items();
            std::sort(resultado.begin(), resultado.end(),
                [](const std::pair<K, V>& a, const std::pair<K, V>& b){
                    return a.first < b.first;
                });
            return resultado;
        }

        /**
         * @brief Inicio del recorrido (el primero insertado).
         * @note Complejidad: O(1).
         */
        iterator begin(){
            return items_.begin();
        }

        /**
         * @brief Fin del recorrido (despues del ultimo insertado).
         * @note Complejidad: O(1).
         */
        iterator end(){
            return items_.end();
        }

        /**
         * @brief Inicio del recorrido de solo lectura.
         * @note Complejidad: O(1).
         */
        const_iterator begin() const{
            return items_.cbegin();
        }

        /**
         * @brief Fin del recorrido de solo lectura.
         * @note Complejidad: O(1).
         */
        const_iterator end() const{
            return items_.cend();
        }

};

/**
 * @brief Imprime el diccionario en orden de insercion como {a: 1, b: 2}.
 * @tparam K Tipo de las llaves.
 * @tparam V Tipo de los valores.
 * @param os Stream de salida.
 * @param diccionario Diccionario a imprimir.
 * @return El mismo stream, para encadenar <<.
 * @note Complejidad: O(n).
 */
template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const Dictionary<K, V>& diccionario) {
    os << "{";
    bool primero = true;
    for (const auto& item : diccionario) {
        if (!primero){
            os << ", ";
        }
        os << item.first << ": " << item.second;
        primero = false;
    }
    os << "}";
    return os;
}

} //namespace structures
