%{
#include <cstdio>
#include <iostream>
int yylex();
void yyerror(const char* s);
extern int yylineno;
extern FILE* yyin;
extern int errores_lexicos;   // viene del scanner (babyduck.l)
%}


%token PROGRAM MAIN END VAR INT FLOAT VOID IF ELSE WHILE DO PRINT READ RETURN
%token NE ID CTE_INT CTE_FLOAT CTE_STRING
%%

/* ---------- Programa ---------- */
programa : PROGRAM ID ';' vars funcs MAIN body END
         ;

funcs : func funcs
      | %empty
      ;

/* ---------- Vars ---------- */
vars : VAR variables
     | %empty
     ;

variables : ids ':' type ';' variables
          | ids ':' type ';'
          ;

ids : ID ',' ids
    | ID
    ;

/* ---------- Type ---------- */
type : INT
     | FLOAT
     ;

/* ---------- Funcs ---------- */
func : typef ID '(' parametro ')' '[' vars body ']' ';'
     ;

typef : VOID
      | type
      ;

parametro : lista_parametro
          | %empty
          ;

lista_parametro : ID ':' type ',' lista_parametro
                | ID ':' type
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
    if (yyparse() == 0 && errores_lexicos == 0) { std::cout << "Programa aceptado\n"; return 0; }
    return 1;                                        // 1 = hubo error
}