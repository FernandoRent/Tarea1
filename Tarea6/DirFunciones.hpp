#pragma once
// Tarea 4: Directorio de Funciones y Tablas de Variables de BabyDuck
// Tarea 6: cada variable guarda tambien su direccion virtual
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Semantica.hpp"

// Una variable: su nombre, su tipo y su direccion virtual
struct Variable {
    std::string nombre;
    Tipo tipo;
    int dir;
};

// Tabla de variables de UN scope (el global o el de una funcion).
// unordered_map = tabla hash: buscar/insertar por nombre en O(1) promedio,
// y no permite nombres repetidos (justo la regla del lenguaje).
class TablaVariables {
public:
    bool existe(const std::string& nombre) const {
        return vars.count(nombre) > 0;
    }

    // Regresa false si ya existia (variable doblemente declarada)
    bool agregar(const std::string& nombre, Tipo tipo, int dir) {
        if (existe(nombre)) return false;
        vars[nombre] = Variable{nombre, tipo, dir};
        return true;
    }

    // Regresa nullptr si no existe
    const Variable* buscar(const std::string& nombre) const {
        auto it = vars.find(nombre);
        return it == vars.end() ? nullptr : &it->second;
    }

    void imprimir() const {
        for (const auto& par : vars)
            std::cout << "    " << par.first << " : " << nombreTipo(par.second.tipo)
                      << " -> " << par.second.dir << "\n";
    }

private:
    std::unordered_map<std::string, Variable> vars;
};

// Cuantas casillas usa una funcion de cada tipo (para ERA)
struct Tamano {
    int localInt = 0, localFloat = 0;
    int tempInt = 0, tempFloat = 0, tempBool = 0;
};

// Una funcion: tipo de retorno, firma (parametros en orden) y su tabla
struct Funcion {
    std::string nombre;
    Tipo retorno;
    std::vector<Tipo> parametros;   // vector: el orden importa para validar llamadas
    TablaVariables tabla;           // parametros + variables locales
    int inicio = -1;                // Tarea 6: numero del primer cuadruplo de la funcion
    int dirRetorno = -1;            // Tarea 6: direccion global donde deja su valor (si no es void)
    Tamano tam;                     // Tarea 6: casillas que pide la funcion (ERA)
};

// Directorio de funciones: nombre -> Funcion.
// El programa (scope global) se guarda como una funcion mas, tipo void.
class DirFunciones {
public:
    bool existe(const std::string& nombre) const {
        return funcs.count(nombre) > 0;
    }

    // Regresa false si ya existia (funcion doblemente declarada)
    bool agregar(const std::string& nombre, Tipo retorno) {
        if (existe(nombre)) return false;
        Funcion f;
        f.nombre = nombre;
        f.retorno = retorno;
        funcs[nombre] = f;
        return true;
    }

    // Solo se llama con nombres que ya existen
    Funcion& buscar(const std::string& nombre) {
        return funcs.at(nombre);
    }

    void imprimir() const {
        for (const auto& par : funcs) {
            const Funcion& f = par.second;
            std::cout << f.nombre << " (" << nombreTipo(f.retorno) << ", "
                      << f.parametros.size() << " parametros";
            if (f.inicio != -1) std::cout << ", inicia en " << f.inicio;
            if (f.dirRetorno != -1) std::cout << ", regresa en " << f.dirRetorno;
            std::cout << ")\n";
            if (f.inicio != -1)
                std::cout << "    ERA -> locales int " << f.tam.localInt
                          << ", float " << f.tam.localFloat
                          << " | temporales int " << f.tam.tempInt
                          << ", float " << f.tam.tempFloat
                          << ", bool " << f.tam.tempBool << "\n";
            f.tabla.imprimir();
        }
    }

private:
    std::unordered_map<std::string, Funcion> funcs;
};
