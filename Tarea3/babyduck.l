%option noyywrap
%option nounput
%option noinput
%option yylineno

%{
#include <iostream> 
#include "parser.hpp"
int errores_lexicos = 0;   // cuenta los caracteres invalidos
%}

%%
[ \t\r\n]+          { /* ignorar */ }

"program"           { return PROGRAM; }
"int"           { return INT; }
"float"           { return FLOAT; }
"void"           { return VOID; }
"var"           { return VAR; }
"if"           { return IF; }
"else"           { return ELSE; }
"print"           { return PRINT; }
"end"           { return END; }
"while"           { return WHILE; }
"do"           { return DO; }
"read"           { return READ; }
"main"           { return MAIN; }
"return"           { return RETURN; }


"!="                { return NE; }

[0-9]+\.[0-9]+      { return CTE_FLOAT; }
[0-9]+              {return CTE_INT; }
\"[^\"\n]\"            {return CTE_STRING; }
[A-Za-z][A-Za-z0-9_]*  {return ID; }

[-+*/=<>;,:(){}\[\]]  { return yytext[0]; }

.                   { errores_lexicos++;
                      std::cerr << "Caracter invalido '" << yytext
                                << "' en linea " << yylineno << "\n"; }


%%