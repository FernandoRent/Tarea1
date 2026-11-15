#pragma once
// Tarea 4: Directorio de Funciones y Tablas de Variables de BabyDuck
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Semantica.hpp"

// Una variable: su nombre y su tipo
struct Variable {
    std::string nombre;
    Tipo tipo;
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
    bool agregar(const std::string& nombre, Tipo tipo) {
        if (existe(nombre)) return false;
        vars[nombre] = Variable{nombre, tipo};
        return true;
    }

    // Regresa nullptr si no existe
    const Variable* buscar(const std::string& nombre) const {
        auto it = vars.find(nombre);
        return it == vars.end() ? nullptr : &it->second;
    }

    void imprimir() const {
        for (const auto& par : vars)
            std::cout << "    " << par.first << " : " << nombreTipo(par.second.tipo) << "\n";
    }

private:
    std::unordered_map<std::string, Variable> vars;
};

// Una funcion: tipo de retorno, firma (parametros en orden) y su tabla
struct Funcion {
    std::string nombre;
    Tipo retorno;
    std::vector<Tipo> parametros;   // vector: el orden importa para validar llamadas
    TablaVariables tabla;           // parametros + variables locales
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
        funcs[nombre] = Funcion{nombre, retorno, {}, {}};
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
                      << f.parametros.size() << " parametros)\n";
            f.tabla.imprimir();
        }
    }

private:
    std::unordered_map<std::string, Funcion> funcs;
};
