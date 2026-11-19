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

GeneradorCuadruplos gen;                  // Tarea 5: pilas + fila de cuadruplos (Tarea 6: + memoria)

/* Tarea 6: llamadas en curso. Pila porque una llamada puede ir dentro de
   los argumentos de otra: f(g(x), y) */
structures::Stack<std::string> pLlamadas;   // funcion que se esta llamando
structures::Stack<int>         pArgs;       // cuantos argumentos lleva

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

/* Meter un operando (su direccion) y su tipo (las dos pilas van a la par) */
void meterOperando(int dir, Tipo tipo) {
    gen.pOperandos.push(dir);
    gen.pTipos.push(tipo);
}

/* Tarea 6: segmento donde va una variable nueva (global o local) */
Segmento segmentoActual() {
    return funcActual == programaNombre ? Segmento::GLOBAL : Segmento::LOCAL;
}

/* Tarea 6: direccion de una constante (la da de alta si es nueva).
   Regresa false si ya no caben mas constantes de ese tipo. */
bool meterConstante(const std::string& valor, Tipo tipo) {
    int dir = gen.mem.constante(valor, tipo);
    if (dir < 0) { errorSemantico("demasiadas constantes"); return false; }
    meterOperando(dir, tipo);
    return true;
}

/* Sacar el operando de arriba (cuando un estatuto ya uso la expresion) */
void sacarOperando() {
    gen.pOperandos.pop();
    gen.pTipos.pop();
}

/* Tarea 6: la condicion de if/while ya esta arriba de la pila; debe ser bool.
   Genera (GOTOF, cond, , ___) y guarda su numero en la pila de saltos. */
bool generarGotoF() {
    int cond = gen.pOperandos.pop();
    Tipo t = gen.pTipos.pop();
    if (t != Tipo::BOOL) {
        errorSemantico(std::string("la condicion debe ser bool, no ") + nombreTipo(t));
        return false;
    }
    gen.generar("GOTOF", cond, VACIO, VACIO);   /* el destino se rellena despues */
    gen.pSaltos.push(gen.siguiente() - 1);
    return true;
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

    int der = gen.pOperandos.pop();  Tipo tDer = gen.pTipos.pop();
    int izq = gen.pOperandos.pop();  Tipo tIzq = gen.pTipos.pop();

    Tipo tRes = cubo.resultado(tIzq, tDer, textoAOp(op));
    if (tRes == Tipo::ERROR) {
        errorSemantico("tipos incompatibles: " + std::string(nombreTipo(tIzq)) + " "
                       + op + " " + nombreTipo(tDer));
        return false;
    }
    int temp = gen.nuevoTemporal(tRes);
    if (temp < 0) { errorSemantico("demasiados temporales"); return false; }
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
                 /* Tarea 6: el primer cuadruplo salta a main (se rellena despues) */
                 gen.generar("GOTO", VACIO, VACIO, VACIO);
             }
           ';' vars funcs MAIN
             {   /* Tarea 6: rellenar el GOTO main; main usa sus propios temporales */
                 gen.rellenar(1, gen.siguiente());
                 gen.mem.reiniciarLocales();
             }
           body END
             {   /* Tarea 6: fin del programa */
                 gen.generar("END", VACIO, VACIO, VACIO);
             }
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
               if (dir.buscar(funcActual).tabla.existe(nombre)) {
                   errorSemantico("variable doblemente declarada: " + nombre);
                   YYABORT;
               }
               int d = gen.mem.pedir(segmentoActual(), $3);   /* Tarea 6: su direccion */
               if (d < 0) { errorSemantico("demasiadas variables"); YYABORT; }
               dir.buscar(funcActual).tabla.agregar(nombre, $3, d);
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
             gen.mem.reiniciarLocales();   /* Tarea 6: locales y temporales desde el inicio */
             if ($1 != Tipo::VOID) {
                 /* Tarea 6: direccion global donde la funcion deja su valor */
                 int d = gen.mem.pedir(Segmento::GLOBAL, $1);
                 if (d < 0) { errorSemantico("demasiadas variables"); free($2); YYABORT; }
                 dir.buscar($2).dirRetorno = d;
             }
             free($2);
         }
       '(' parametro ')' '[' vars
         {   /* Tarea 6: aqui empieza el codigo de la funcion */
             dir.buscar(funcActual).inicio = gen.siguiente();
         }
       body ']' ';'
         {   /* Tarea 6: ENDFUNC y guardar cuantas casillas uso (para ERA) */
             gen.generar("ENDFUNC", VACIO, VACIO, VACIO);
             Tamano& t = dir.buscar(funcActual).tam;
             t.localInt   = gen.mem.usadas(Segmento::LOCAL, Tipo::INT);
             t.localFloat = gen.mem.usadas(Segmento::LOCAL, Tipo::FLOAT);
             t.tempInt    = gen.mem.usadas(Segmento::TEMPORAL, Tipo::INT);
             t.tempFloat  = gen.mem.usadas(Segmento::TEMPORAL, Tipo::FLOAT);
             t.tempBool   = gen.mem.usadas(Segmento::TEMPORAL, Tipo::BOOL);
             /* PN6: al terminar la funcion, regresar al scope global */
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
            if (f.tabla.existe($1)) {
                errorSemantico(std::string("parametro repetido: ") + $1);
                free($1);
                YYABORT;
            }
            int d = gen.mem.pedir(Segmento::LOCAL, $3);   /* Tarea 6: los parametros son locales */
            if (d < 0) { errorSemantico("demasiadas variables"); free($1); YYABORT; }
            f.tabla.agregar($1, $3, d);
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
               meterOperando(v->dir, v->tipo);
               gen.pOperadores.push("=");
           }
         expresion ';'
           {   /* PN: sacar valor, variable y "="; validar con el cubo; generar (=, valor, , var) */
               gen.pOperadores.pop();
               int valor = gen.pOperandos.pop();  Tipo tValor = gen.pTipos.pop();
               int var   = gen.pOperandos.pop();  Tipo tVar   = gen.pTipos.pop();
               if (cubo.resultado(tVar, tValor, Op::ASIG) == Tipo::ERROR) {
                   errorSemantico("no se puede asignar " + std::string(nombreTipo(tValor))
                                  + " a " + $1 + " (" + nombreTipo(tVar) + ")");
                   free($1);
                   YYABORT;
               }
               gen.generar("=", valor, VACIO, var);
               free($1);
           }
       ;

/* ---------- Condition ---------- */
condition : cond_inicio statement ';'
              {   /* PN: sin else, el GOTOF salta aqui (al final del if) */
                  gen.rellenar(gen.pSaltos.pop(), gen.siguiente());
              }
          | cond_inicio statement ELSE
              {   /* PN: GOTO al final (salta el else); el GOTOF salta al inicio del else */
                  gen.generar("GOTO", VACIO, VACIO, VACIO);
                  int falso = gen.pSaltos.pop();
                  gen.pSaltos.push(gen.siguiente() - 1);
                  gen.rellenar(falso, gen.siguiente());
              }
            statement ';'
              {   /* PN: el GOTO salta aqui (al final del if) */
                  gen.rellenar(gen.pSaltos.pop(), gen.siguiente());
              }
          ;

/* parte comun del if (asi Bison no ve dos acciones iguales en conflicto) */
cond_inicio : IF '(' expresion ')'
                {   /* PN: la condicion debe ser bool; GOTOF pendiente */
                    if (!generarGotoF()) YYABORT;
                }
            ;

/* ---------- Cycle ---------- */
cycle : WHILE
          {   /* PN: guardar a donde regresar (inicio de la condicion) */
              gen.pSaltos.push(gen.siguiente());
          }
        '(' expresion ')'
          {   /* PN: la condicion debe ser bool; GOTOF pendiente */
              if (!generarGotoF()) YYABORT;
          }
        DO statement ';'
          {   /* PN: GOTO al inicio de la condicion; el GOTOF salta aqui (fuera del ciclo) */
              int falso   = gen.pSaltos.pop();
              int regreso = gen.pSaltos.pop();
              gen.generar("GOTO", VACIO, VACIO, regreso);
              gen.rellenar(falso, gen.siguiente());
          }
      ;

/* ---------- Print ---------- */
print : PRINT '(' print_list ')' ';'
      ;

print_list : elemento ',' print_list
           | elemento
           ;

elemento : expresion
             {   /* PN: sacar el resultado y generar (print, valor, , ) */
                 int valor = gen.pOperandos.pop();
                 if (gen.pTipos.pop() == Tipo::VOID) {
                     errorSemantico("una funcion void no regresa valor para imprimir");
                     YYABORT;
                 }
                 gen.generar("print", valor, VACIO, VACIO);
             }
         | CTE_STRING
             {   /* PN: el letrero es una constante string (10000...) */
                 int d = gen.mem.constante($1, Tipo::STRING);
                 free($1);
                 if (d < 0) { errorSemantico("demasiadas constantes"); YYABORT; }
                 gen.generar("print", d, VACIO, VACIO);
             }
         ;

/* ---------- Read ---------- */
read : READ '(' ID ')' ';'
       {   /* PN: la variable debe existir; generar (read, , , var) */
           const Variable* v = buscarVariable($3);
           if (v == nullptr) {
               errorSemantico(std::string("variable no declarada: ") + $3);
               free($3);
               YYABORT;
           }
           gen.generar("read", VACIO, VACIO, v->dir);
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
               /* Tarea 6: ERA (pedir espacio para la funcion) y empezar a contar argumentos */
               gen.generar("ERA", VACIO, VACIO, dir.buscar($1).inicio);
               pLlamadas.push($1);
               pArgs.push(0);
           }
         '(' llamada_f ')'
           {   /* quitar el fondo falso */
               gen.pOperadores.pop();
               Funcion& f = dir.buscar($1);
               int k = pArgs.pop();
               pLlamadas.pop();
               /* Tarea 6: deben venir todos los argumentos */
               if (k != (int)f.parametros.size()) {
                   errorSemantico("la funcion " + f.nombre + " espera "
                                  + std::to_string(f.parametros.size()) + " argumentos y recibio "
                                  + std::to_string(k));
                   free($1);
                   YYABORT;
               }
               /* Tarea 6: GOSUB salta al inicio de la funcion */
               gen.generar("GOSUB", VACIO, VACIO, f.inicio);
               if (f.retorno == Tipo::VOID) {
                   meterOperando(VACIO, Tipo::VOID);   /* no tiene valor */
               } else {
                   /* copiar el valor que dejo la funcion a un temporal */
                   int temp = gen.nuevoTemporal(f.retorno);
                   if (temp < 0) { errorSemantico("demasiados temporales"); free($1); YYABORT; }
                   gen.generar("=", f.dirRetorno, VACIO, temp);
                   meterOperando(temp, f.retorno);
               }
               free($1);
           }
       ;

llamada_f : funcion
          | %empty
          ;

funcion : argumento ',' funcion
        | argumento
        ;

argumento : expresion
              {   /* Tarea 6: validar el argumento contra el parametro y generar PARAM */
                  Funcion& f = dir.buscar(pLlamadas.peek());
                  int k = pArgs.pop() + 1;            /* numero de este argumento (desde 1) */
                  pArgs.push(k);
                  int valor = gen.pOperandos.pop();  Tipo t = gen.pTipos.pop();
                  if (k > (int)f.parametros.size()) {
                      errorSemantico("demasiados argumentos para " + f.nombre);
                      YYABORT;
                  }
                  Tipo tParam = f.parametros[k - 1];
                  if (cubo.resultado(tParam, t, Op::ASIG) == Tipo::ERROR) {
                      errorSemantico("el argumento " + std::to_string(k) + " de " + f.nombre
                                     + " debe ser " + nombreTipo(tParam) + ", no " + nombreTipo(t));
                      YYABORT;
                  }
                  gen.generar("PARAM", valor, VACIO, k);
              }
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
               int a = gen.pOperandos.pop();  Tipo t = gen.pTipos.pop();
               int cero = gen.mem.constante("0", Tipo::INT);
               int temp = gen.nuevoTemporal(t);
               if (cero < 0 || temp < 0) { errorSemantico("sin direcciones libres"); YYABORT; }
               gen.generar("-", cero, a, temp);
               meterOperando(temp, t);
           }
       | '-' CTE_INT     /* constante negativa: se guarda directo como "-5" */
           {   bool ok = meterConstante("-" + std::string($2), Tipo::INT);   free($2);
               if (!ok) YYABORT; }
       | '-' CTE_FLOAT
           {   bool ok = meterConstante("-" + std::string($2), Tipo::FLOAT); free($2);
               if (!ok) YYABORT; }
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
               meterOperando(v->dir, v->tipo);   /* Tarea 6: su direccion, no su nombre */
               free($1);
           }
         ;

/* ---------- CTE ---------- */
/* PN: meter la direccion de la constante (Tarea 6: tabla de constantes) */
cte : CTE_INT   { bool ok = meterConstante($1, Tipo::INT);   free($1); if (!ok) YYABORT; }
    | CTE_FLOAT { bool ok = meterConstante($1, Tipo::FLOAT); free($1); if (!ok) YYABORT; }
    ;

/* ---------- Return ---------- */
return : RETURN expresion ';'
           {   /* Tarea 6: solo en funciones que regresan valor; el tipo debe cuadrar */
               Funcion& f = dir.buscar(funcActual);
               int valor = gen.pOperandos.pop();  Tipo t = gen.pTipos.pop();
               if (f.retorno == Tipo::VOID) {
                   errorSemantico("return en " + f.nombre + ", que no regresa valor");
                   YYABORT;
               }
               if (cubo.resultado(f.retorno, t, Op::ASIG) == Tipo::ERROR) {
                   errorSemantico("la funcion " + f.nombre + " regresa " + nombreTipo(f.retorno)
                                  + ", no " + nombreTipo(t));
                   YYABORT;
               }
               gen.generar("RETURN", valor, VACIO, f.dirRetorno);
           }
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
        gen.mem.imprimirConstantes();                    // Tarea 6: constantes y su direccion
        gen.imprimir();                                  // Tarea 5: desplegar la fila
        return 0;
    }
    return 1;                                        // 1 = hubo error
}