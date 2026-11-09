/* parser.hpp necesita conocer "Tipo" porque va dentro del %union */
%code requires {
#include "DirFunciones.hpp"
}

%{
#include <cstdio>
#include <iostream>
int yylex();
void yyerror(const char* s);
extern int yylineno;
extern FILE* yyin;
extern int errores_lexicos;   // viene del scanner (babyduck.l)

#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "DirFunciones.hpp"

/* ---- Estado semantico (Tarea 4) ---- */
DirFunciones dir;                         // directorio de funciones
std::string programaNombre;               // nombre del scope global
std::string funcActual;                   // scope donde estamos ahorita
std::vector<std::string> idsPendientes;   // ids de "a, b, c : int;" antes de saber el tipo

void errorSemantico(const std::string& msg) {
    std::cerr << "Error semantico en linea " << yylineno << ": " << msg << "\n";
}
%}


/* Valores que pueden cargar los tokens y las reglas */
%union {
    char* texto;   /* nombre de un ID (lo llena el scanner) */
    Tipo  tipo;    /* tipo que regresan las reglas type y typef */
}

%token PROGRAM MAIN END VAR INT FLOAT VOID IF ELSE WHILE DO PRINT READ RETURN
%token NE CTE_INT CTE_FLOAT CTE_STRING
%token <texto> ID
%type  <tipo>  type typef
%%

/* ---------- Programa ---------- */
programa : PROGRAM ID
             {   /* PN1: crear el scope global en el directorio */
                 programaNombre = $2;
                 funcActual = $2;
                 dir.agregar($2, Tipo::VOID);
                 free($2);
             }
           ';' vars funcs MAIN body END
         ;

funcs : func funcs
      | %empty
      ;

/* ---------- Vars ---------- */
vars : VAR variables
     | %empty
     ;

variables : decl variables
          | decl
          ;

decl : ids ':' type ';'
       {   /* PN3: meter cada id a la tabla del scope actual */
           for (const std::string& nombre : idsPendientes) {
               if (!dir.buscar(funcActual).tabla.agregar(nombre, $3)) {
                   errorSemantico("variable doblemente declarada: " + nombre);
                   YYABORT;
               }
           }
           idsPendientes.clear();
       }
     ;

ids : ID { idsPendientes.push_back($1); free($1); } ',' ids   /* PN2: juntar ids */
    | ID { idsPendientes.push_back($1); free($1); }
    ;

/* ---------- Type ---------- */
type : INT   { $$ = Tipo::INT; }
     | FLOAT { $$ = Tipo::FLOAT; }
     ;

/* ---------- Funcs ---------- */
func : typef ID
         {   /* PN4: dar de alta la funcion y cambiar al scope de la funcion */
             if (!dir.agregar($2, $1)) {
                 errorSemantico(std::string("funcion doblemente declarada: ") + $2);
                 free($2);
                 YYABORT;
             }
             funcActual = $2;
             free($2);
         }
       '(' parametro ')' '[' vars body ']' ';'
         {   /* PN6: al terminar la funcion, regresar al scope global */
             funcActual = programaNombre;
         }
     ;

typef : VOID { $$ = Tipo::VOID; }
      | type { $$ = $1; }
      ;

parametro : lista_parametro
          | %empty
          ;

lista_parametro : param ',' lista_parametro
                | param
                ;

param : ID ':' type
        {   /* PN5: el parametro va a la tabla de la funcion y a su firma */
            Funcion& f = dir.buscar(funcActual);
            if (!f.tabla.agregar($1, $3)) {
                errorSemantico(std::string("parametro repetido: ") + $1);
                free($1);
                YYABORT;
            }
            f.parametros.push_back($3);
            free($1);
        }
      ;

/* ---------- Body ---------- */
body : '{' statements '}'
     ;

statements : statement statements
           | %empty
           ;

/* ---------- Statement ---------- */
statement : assign
          | return
          | condition
          | cycle
          | f_call ';'
          | print
          | read
          | '[' statements ']'
          ;

/* ---------- Assign ---------- */
assign : ID '=' expresion ';'
       ;

/* ---------- Condition ---------- */
condition : IF '(' expresion ')' statement ';'
          | IF '(' expresion ')' statement ELSE statement ';'
          ;

/* ---------- Cycle ---------- */
cycle : WHILE '(' expresion ')' DO statement ';'
      ;

/* ---------- Print ---------- */
print : PRINT '(' print_list ')' ';'
      ;

print_list : elemento ',' print_list
           | elemento
           ;

elemento : expresion
         | CTE_STRING
         ;

/* ---------- Read ---------- */
read : READ '(' ID ')' ';'
     ;

/* ---------- F_Call (sin ';' para poder usarla dentro de FACTOR) ---------- */
f_call : ID '(' llamada_f ')'
       ;

llamada_f : funcion
          | %empty
          ;

funcion : expresion ',' funcion
        | expresion
        ;

/* ---------- Expresion ---------- */
expresion : exp
          | exp '>' exp
          | exp '<' exp
          | exp NE exp
          ;

/* ---------- EXP ---------- */
exp : exp '+' termino
    | exp '-' termino
    | termino
    ;

/* ---------- Termino ---------- */
termino : termino '*' factor
        | termino '/' factor
        | factor
        ;

/* ---------- Factor ---------- */
factor : '(' expresion ')'
       | ID
       | cte
       | f_call
       | '+' ID
       | '-' ID
       | '-' cte
       | '+' cte
       ;

/* ---------- CTE ---------- */
cte : CTE_INT
    | CTE_FLOAT
    ;

/* ---------- Return ---------- */
return : RETURN expresion ';'
       ;

%%

void yyerror(const char* s) {
    std::cerr << "Error de sintaxis en linea " << yylineno << ": " << s << "\n";
}

int main(int argc, char* argv[]) {
    if (argc > 1) yyin = std::fopen(argv[1], "r");   // leer el programa de un archivo
    if (!yyin) { std::cerr << "No se pudo abrir el archivo\n"; return 2; }
    if (yyparse() == 0 && errores_lexicos == 0) {
        std::cout << "Programa aceptado\n";
        if (argc > 2 && std::strcmp(argv[2], "-t") == 0) dir.imprimir();   // ver las tablas
        return 0;
    }
    return 1;                                        // 1 = hubo error
}