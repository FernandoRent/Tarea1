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
#include "Cuadruplos.hpp"

/* ---- Estado semantico (Tarea 4) ---- */
DirFunciones dir;                         // directorio de funciones
std::string programaNombre;               // nombre del scope global
std::string funcActual;                   // scope donde estamos ahorita
std::vector<std::string> idsPendientes;   // ids de "a, b, c : int;" antes de saber el tipo

GeneradorCuadruplos gen;                  // Tarea 5: pilas + fila de cuadruplos

CuboSemantico cubo;                       // para validar cada operacion

void errorSemantico(const std::string& msg) {
    std::cerr << "Error semantico en linea " << yylineno << ": " << msg << "\n";
}

/* Busca una variable: primero en el scope actual, luego en el global */
const Variable* buscarVariable(const std::string& nombre) {
    const Variable* v = dir.buscar(funcActual).tabla.buscar(nombre);
    if (v == nullptr) v = dir.buscar(programaNombre).tabla.buscar(nombre);
    return v;
}

/* Meter un operando y su tipo (las dos pilas van a la par) */
void meterOperando(const std::string& nombre, Tipo tipo) {
    gen.pOperandos.push(nombre);
    gen.pTipos.push(tipo);
}

/* Sacar el operando de arriba (cuando un estatuto ya uso la expresion) */
void sacarOperando() {
    gen.pOperandos.pop();
    gen.pTipos.pop();
}

/* Si el operador de arriba de la pila es op1, op2 u op3, resolverlo:
   sacar 2 operandos, validar con el cubo, generar el cuadruplo
   y meter el temporal con el tipo resultante.
   Regresa false si los tipos no se pueden operar. */
bool resolverSi(const std::string& op1, const std::string& op2,
                const std::string& op3 = "") {
    if (gen.pOperadores.isEmpty()) return true;
    std::string op = gen.pOperadores.peek();
    if (op != op1 && op != op2 && op != op3) return true;
    gen.pOperadores.pop();

    std::string der = gen.pOperandos.pop();  Tipo tDer = gen.pTipos.pop();
    std::string izq = gen.pOperandos.pop();  Tipo tIzq = gen.pTipos.pop();

    Tipo tRes = cubo.resultado(tIzq, tDer, textoAOp(op));
    if (tRes == Tipo::ERROR) {
        errorSemantico("tipos incompatibles: " + std::string(nombreTipo(tIzq)) + " "
                       + op + " " + nombreTipo(tDer));
        return false;
    }
    std::string temp = gen.nuevoTemporal();
    gen.generar(op, izq, der, temp);
    meterOperando(temp, tRes);
    return true;
}
%}


/* Valores que pueden cargar los tokens y las reglas */
%union {
    char* texto;   /* nombre de un ID (lo llena el scanner) */
    Tipo  tipo;    /* tipo que regresan las reglas type y typef */
}

%token PROGRAM MAIN END VAR INT FLOAT VOID IF ELSE WHILE DO PRINT READ RETURN
%token NE
%token <texto> ID CTE_INT CTE_FLOAT CTE_STRING
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
          | f_call ';' { sacarOperando(); }   /* la llamada sola no usa su valor */
          | print
          | read
          | '[' statements ']'
          ;

/* ---------- Assign ---------- */
assign : ID '='
           {   /* PN: la variable debe existir; se mete con su tipo y se mete "=" */
               const Variable* v = buscarVariable($1);
               if (v == nullptr) {
                   errorSemantico(std::string("variable no declarada: ") + $1);
                   free($1);
                   YYABORT;
               }
               meterOperando($1, v->tipo);
               gen.pOperadores.push("=");
           }
         expresion ';'
           {   /* PN: sacar valor, variable y "="; validar con el cubo; generar (=, valor, , var) */
               gen.pOperadores.pop();
               std::string valor = gen.pOperandos.pop();  Tipo tValor = gen.pTipos.pop();
               std::string var   = gen.pOperandos.pop();  Tipo tVar   = gen.pTipos.pop();
               if (cubo.resultado(tVar, tValor, Op::ASIG) == Tipo::ERROR) {
                   errorSemantico("no se puede asignar " + std::string(nombreTipo(tValor))
                                  + " a " + var + " (" + nombreTipo(tVar) + ")");
                   free($1);
                   YYABORT;
               }
               gen.generar("=", valor, "", var);
               free($1);
           }
       ;

/* ---------- Condition ---------- */
condition : cond_inicio statement ';'
          | cond_inicio statement ELSE statement ';'
          ;

/* parte comun del if (asi Bison no ve dos acciones iguales en conflicto) */
cond_inicio : IF '(' expresion ')' { sacarOperando(); }   /* GOTOF es de otra entrega */
            ;

/* ---------- Cycle ---------- */
cycle : WHILE '(' expresion ')' { sacarOperando(); } DO statement ';'
      ;

/* ---------- Print ---------- */
print : PRINT '(' print_list ')' ';'
      ;

print_list : elemento ',' print_list
           | elemento
           ;

elemento : expresion
             {   /* PN: sacar el resultado y generar (print, valor, , ) */
                 std::string valor = gen.pOperandos.pop();
                 gen.pTipos.pop();
                 gen.generar("print", valor, "", "");
             }
         | CTE_STRING
             {   /* PN: los letreros se imprimen tal cual */
                 gen.generar("print", $1, "", "");
                 free($1);
             }
         ;

/* ---------- Read ---------- */
read : READ '(' ID ')' ';'
       {   /* PN: la variable debe existir; generar (read, , , var) */
           if (buscarVariable($3) == nullptr) {
               errorSemantico(std::string("variable no declarada: ") + $3);
               free($3);
               YYABORT;
           }
           gen.generar("read", "", "", $3);
           free($3);
       }
     ;

/* ---------- F_Call (sin ';' para poder usarla dentro de FACTOR) ---------- */
f_call : ID
           {   /* PN: la funcion debe existir; "(" = fondo falso para sus argumentos */
               if (!dir.existe($1)) {
                   errorSemantico(std::string("funcion no declarada: ") + $1);
                   free($1);
                   YYABORT;
               }
               gen.pOperadores.push("(");
           }
         '(' llamada_f ')'
           {   /* quitar el fondo falso; la llamada vale lo que regresa la funcion */
               gen.pOperadores.pop();
               meterOperando($1, dir.buscar($1).retorno);
               free($1);
           }
       ;

llamada_f : funcion
          | %empty
          ;

funcion : argumento ',' funcion
        | argumento
        ;

argumento : expresion { sacarOperando(); }   /* los parametros son de otra entrega */
          ;

/* ---------- Expresion ---------- */
expresion : exp
          | exp '>' { gen.pOperadores.push(">"); }               /* PN: meter el operador */
                exp { if (!resolverSi(">", "<", "!=")) YYABORT; }  /* PN: resolver relacional */
          | exp '<' { gen.pOperadores.push("<"); }
                exp { if (!resolverSi(">", "<", "!=")) YYABORT; }
          | exp NE  { gen.pOperadores.push("!="); }
                exp { if (!resolverSi(">", "<", "!=")) YYABORT; }
          ;

/* ---------- EXP ---------- */
exp : exp '+' { gen.pOperadores.push("+"); }            /* PN: meter el operador */
          termino { if (!resolverSi("+", "-")) YYABORT; }   /* PN: resolver + o - */
    | exp '-' { gen.pOperadores.push("-"); }
          termino { if (!resolverSi("+", "-")) YYABORT; }
    | termino
    ;

/* ---------- Termino ---------- */
termino : termino '*' { gen.pOperadores.push("*"); }       /* PN: meter el operador */
              factor { if (!resolverSi("*", "/")) YYABORT; }  /* PN: resolver * o / */
        | termino '/' { gen.pOperadores.push("/"); }
              factor { if (!resolverSi("*", "/")) YYABORT; }
        | factor
        ;

/* ---------- Factor ---------- */
factor : '(' { gen.pOperadores.push("("); }   /* PN: fondo falso */
         expresion ')' { gen.pOperadores.pop(); }  /* PN: quitar fondo falso */
       | variable
       | cte
       | f_call
       | '+' variable                 /* +a es lo mismo que a */
       | '-' variable
           {   /* PN: menos unario -> (-, 0, a, t) */
               std::string a = gen.pOperandos.pop();  Tipo t = gen.pTipos.pop();
               std::string temp = gen.nuevoTemporal();
               gen.generar("-", "0", a, temp);
               meterOperando(temp, t);
           }
       | '-' cte
           {   /* constante negativa: se guarda directo como "-5" */
               std::string c = gen.pOperandos.pop();
               gen.pOperandos.push("-" + c);
           }
       | '+' cte
       ;

/* PN: una variable usada en una expresion debe estar declarada */
variable : ID
           {
               const Variable* v = buscarVariable($1);
               if (v == nullptr) {
                   errorSemantico(std::string("variable no declarada: ") + $1);
                   free($1);
                   YYABORT;
               }
               meterOperando($1, v->tipo);
               free($1);
           }
         ;

/* ---------- CTE ---------- */
cte : CTE_INT   { meterOperando($1, Tipo::INT);   free($1); }  /* PN: meter constante */
    | CTE_FLOAT { meterOperando($1, Tipo::FLOAT); free($1); }
    ;

/* ---------- Return ---------- */
return : RETURN expresion ';' { sacarOperando(); }   /* return es de otra entrega */
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
        gen.imprimir();                                  // Tarea 5: desplegar la fila
        return 0;
    }
    return 1;                                        // 1 = hubo error
}