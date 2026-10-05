%{
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include "structures/Stack.hpp"
#include "structures/Queue.hpp"
#include "Cuadruplo.hpp"

int yylex();
void yyerror(const char* msg);
extern int yylineno;

structures::Stack<std::string> pilaOperandos;
structures::Queue<Cuadruplo> filaCuadruplos;
int contadorTemp = 1;

void generar(const std::string& op) {
    // Al desapilar: el primero en salir es el operando DERECHO
    std::string der = pilaOperandos.pop();
    std::string izq = pilaOperandos.pop();

    std::string temp = "t" + std::to_string(contadorTemp++);
    filaCuadruplos.enqueue({op, izq, der, temp});
    pilaOperandos.push(temp);
}
%}

%union {
    char* texto;
}

%token <texto> ID NUM

%left '+' '-'
%left '*' '/'

%%

programa:
      %empty
    | programa sentencia
    ;

sentencia:
      ID '=' expr ';' {
          std::string res = pilaOperandos.pop();
          filaCuadruplos.enqueue({"=", res, "-", std::string($1)});
          free($1);
      }
    | error ';' {
          pilaOperandos.clear();
          yyerrok;
      }
    ;

expr:
      expr '+' expr      { generar("+"); }
    | expr '-' expr      { generar("-"); }
    | expr '*' expr      { generar("*"); }
    | expr '/' expr      { generar("/"); }
    | '(' expr ')'       { /* No genera acción, conserva el valor ya evaluado en la pila */ }
    | NUM {
          pilaOperandos.push(std::string($1));
          free($1);
      }
    | ID {
          pilaOperandos.push(std::string($1));
          free($1);
      }
    ;

%%

void yyerror(const char* msg) {
    std::cerr << "Error de sintaxis en linea " << yylineno << ": " << msg << std::endl;
}

int main() {
    yyparse();

    std::cout << "Cuadruplos generados: " << filaCuadruplos.size() << std::endl;
    int idx = 1;
    while (!filaCuadruplos.isEmpty()) {
        std::cout << idx++ << ". " << filaCuadruplos.dequeue() << std::endl;
    }

    return 0;
}
