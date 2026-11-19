#pragma once
// Tarea 6: direcciones virtuales de BabyDuck
// Cada dato (variable, constante o temporal) recibe un numero.
// El rango del numero dice su segmento y su tipo:
//
//   Segmento    int          float        bool         string
//   Global      1000-1999    2000-2999
//   Local       3000-3999    4000-4999
//   Temporal    5000-5999    6000-6999    7000-7999
//   Constante   8000-8999    9000-9999                 10000-10999
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Semantica.hpp"

enum class Segmento { GLOBAL, LOCAL, TEMPORAL, CONSTANTE };

class MemoriaVirtual {
public:
    static const int TAM = 1000;   // casillas por segmento y tipo

    // Pide la siguiente direccion libre de un segmento y tipo.
    // Regresa -1 si ese rango ya se lleno.
    int pedir(Segmento s, Tipo t) {
        int idx = indice(s, t);
        if (idx < 0 || contador[idx] >= TAM) return -1;
        return base(idx) + contador[idx]++;
    }

    // Cada funcion empieza sus locales y temporales desde el inicio del rango
    void reiniciarLocales() {
        for (Segmento s : {Segmento::LOCAL, Segmento::TEMPORAL})
            for (Tipo t : {Tipo::INT, Tipo::FLOAT, Tipo::BOOL}) {
                int idx = indice(s, t);
                if (idx >= 0) contador[idx] = 0;
            }
    }

    // Cuantas casillas se han usado de un segmento y tipo (para ERA)
    int usadas(Segmento s, Tipo t) const {
        int idx = indice(s, t);
        return idx < 0 ? 0 : contador[idx];
    }

    // Tabla de constantes: la misma constante siempre tiene la misma direccion
    int constante(const std::string& valor, Tipo t) {
        auto it = constantes.find(valor);
        if (it != constantes.end()) return it->second;
        int dir = pedir(Segmento::CONSTANTE, t);
        if (dir < 0) return -1;
        constantes[valor] = dir;
        ordenConstantes.push_back(valor);
        return dir;
    }

    void imprimirConstantes() const {
        std::cout << "\nTabla de constantes\n";
        for (const std::string& valor : ordenConstantes)
            std::cout << "  " << constantes.at(valor) << " : " << valor << "\n";
    }

private:
    // 10 rangos en el orden de la tabla de arriba; la base es 1000 * (indice + 1)
    int contador[10] = {0};
    std::unordered_map<std::string, int> constantes;   // valor -> direccion
    std::vector<std::string> ordenConstantes;          // para imprimirlas en orden

    static int base(int idx) { return (idx + 1) * TAM; }

    // Posicion del rango en la tabla; -1 si esa combinacion no existe
    static int indice(Segmento s, Tipo t) {
        switch (s) {
            case Segmento::GLOBAL:
                if (t == Tipo::INT) return 0;
                if (t == Tipo::FLOAT) return 1;
                return -1;
            case Segmento::LOCAL:
                if (t == Tipo::INT) return 2;
                if (t == Tipo::FLOAT) return 3;
                return -1;
            case Segmento::TEMPORAL:
                if (t == Tipo::INT) return 4;
                if (t == Tipo::FLOAT) return 5;
                if (t == Tipo::BOOL) return 6;
                return -1;
            case Segmento::CONSTANTE:
                if (t == Tipo::INT) return 7;
                if (t == Tipo::FLOAT) return 8;
                if (t == Tipo::STRING) return 9;
                return -1;
        }
        return -1;
    }
};
