/**
 *  @file Exceptions.hpp
 *  @brief Excepciones que lanzan las estructuras de datos ante operaciones inválidas
 */ 
#pragma once 
#include <stdexcept>
#include <string>
namespace structures{ 
/**
 *  @brief Esta clase recibe el mensaje que se manda cuando hay un error sirve para que en las siguientes clases los errores se cachen 
 */ 
class StructureException: public std::runtime_error{
public: 
/**
 *  @brief Crea la excepción y guarda el mensaje
 * @param mensaje texto que describe el error
 */ 
    explicit StructureException(const std::string& mensaje)
        :runtime_error(mensaje)
    {}
};

/**
 * @brief Esta clase se lanza cuando hay una pila vacia en una operacion
 */
class EmptyStackException: public StructureException{
public:
/**
 * @brief Crea la excepción y guarda el mensaje 
 * @param mensaje texto que describe el error
 */
    explicit EmptyStackException(const std::string& mensaje="la pila esta vacia")
        :StructureException(mensaje)
    {}
};

/**
 * @brief Esta clase se lanza cuando hay una fila vacia 
 */
class EmptyQueueException: public StructureException{
public:
/**
 * @brief Crea la excepción y guarda el mensaje 
 * @param mensaje texto que describe el error
 */
    explicit EmptyQueueException(const std::string& mensaje="la fila esta vacia")
        :StructureException(mensaje)
    {}
};

/**
 * @brief Esta clase se lanza cuando no hay una llave
 */
class KeyNotFoundException: public StructureException{
public:
/**
 * @brief Crea la excepción y guarda el mensaje 
 * @param mensaje texto que describe el error
 */
    explicit KeyNotFoundException(const std::string& mensaje="llave no encontrada")
        :StructureException(mensaje)
    {}
};
} // namespace structures 