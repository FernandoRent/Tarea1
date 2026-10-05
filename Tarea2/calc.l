%option noyywrap
%option nounput
%option noinput
%option yylineno

%{
#include <cstdio>
#include <cstring>
#include <iostream> 
#include "parser.hpp"
%}

DIGITO  [0-9]
LETRA   [a-zA-Z_]

%%

[ \t\r\n]+             { /* Ignorar espacios y saltos de línea */ }
{DIGITO}+              { yylval.texto = strdup(yytext); return NUM; }
{LETRA}({LETRA}|{DIGITO})* { yylval.texto = strdup(yytext); return ID; }
[-+*/=;()]             { return yytext[0]; }
.                      { std::cerr << "Caracter invalido: " << yytext[0] << " en linea " << yylineno << std::endl; }

%%