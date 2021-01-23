%{
#include <stdio.h>
#include "part3.hpp"
#include "part3.tab.hpp"


void print_lex(char* type);
void print_lex_str();
void print_error();
void assign_value(const char* type);
void assign_value(const char* type);
void assign_value_str();

//extern Line yylval;
%}

%option noyywrap
%option yylineno


id              ([a-zA-Z]+[0-9_a-zA-Z]*)
integernum      ([0-9]+)
realnum         ([0-9]+\.[0-9]+)
str             (\"([^"\n\\]|\\t|\\n|\\\")*\")
comment         (#+[^\r\n]*)
relop           (==|<>|<|<=|>|>=)
addop           (\+|-)
mulop           (\*|\/)
whitespace      ([\r\n\n\t ]+)

%%
{str}           assign_value_str(); return str;
{integernum}    assign_value("integernum"); return integernum;
{realnum}       assign_value("realnum"); return realnum;
{relop}         assign_value("relop"); return relop;
{addop}         assign_value("addop"); return addop;
{mulop}         assign_value("mulop"); return mulop;
\(              assign_value("("); return tk_lp;
\)              assign_value(")"); return tk_rp;
\{              assign_value("{"); return '{';
\}              assign_value("}"); return '}';
\,              assign_value(","); return tk_comma;
\.              assign_value("."); return tk_dot;
\;              assign_value(";"); return ';';
\:              assign_value(":"); return ':';
int             assign_value("int");     return tk_int;    
float           assign_value("float");   return tk_float;
void            assign_value("void");    return tk_void;
write           assign_value("write");   return tk_write;
read            assign_value("read");    return tk_read;
while           assign_value("while");   return tk_while;
do              assign_value("do");      return tk_do;
if              assign_value("if");      return tk_if;
then            assign_value("then");    return tk_then;
else            assign_value("else");    return tk_else;
return          assign_value("return");  return tk_return;
activate        assign_value("activate");return tk_activate;
SCOPE           assign_value("SCOPE");   return tk_scope;
scopes          assign_value("scopes");  return tk_scopes;
=               assign_value("assign");  return tk_assign;
&&              assign_value("and");     return tk_and;
\|\|            assign_value("or");      return tk_or;
!               assign_value("not");     return tk_not;
{id}            assign_value("id");     return id;


{comment}       ; 
{whitespace}    ;
.               print_error();       

%%

void assign_value(const char* type){
    yylval.value = yytext;
}

void assign_value_str(){
    char* value = yytext + 1;
    value[yyleng-2] = '\0';
    yylval.value = value;
}

void print_error(){
    printf("Lexical error: '%s' in line number %d\n",yytext,yylineno);
    exit(Lexical);
}