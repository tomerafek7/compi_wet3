%{
#include <stdio.h>
#include "part3.hpp"
#include "part3.tab.hpp"


void print_lex(char* type);
void print_lex_str();
void print_error();
void assign_value(const char* type);
void assign_type(const char* type);
void assign_value_str();

extern Line* yylval;
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
\(              assign_type("("); return tk_lp;
\)              assign_type(")"); return tk_rp;
\{              assign_type("{"); return '{';
\}              assign_type("}"); return '}';
\,              assign_type(","); return tk_comma;
\.              assign_type("."); return tk_dot;
\;              assign_type(";"); return ';';
\:              assign_type(":"); return ':';
int             assign_type("int");     return tk_int;    
float           assign_type("float");   return tk_float;
void            assign_type("void");    return tk_void;
write           assign_type("write");   return tk_write;
read            assign_type("read");    return tk_read;
while           assign_type("while");   return tk_while;
do              assign_type("do");      return tk_do;
if              assign_type("if");      return tk_if;
then            assign_type("then");    return tk_then;
else            assign_type("else");    return tk_else;
return          assign_type("return");  return tk_return;
activate        assign_type("activate");return tk_activate;
SCOPE           assign_type("SCOPE");   return tk_scope;
scopes          assign_type("scopes");  return tk_scopes;
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
    yylval = makeLine(type, yytext);
}

void assign_type(const char* type){
    yylval = makeLine(type, NULL);
}

void assign_value_str(){
    const char* type = "str";
    char* value = yytext + 1;
    value[yyleng-2] = '\0';
    yylval = makeLine(type, value);
}

//void print_lex(char* type){
//    printf("<%s,%s>",type,yytext);
//}
//
//void print_lex_str(){
//    int idx = 1;
//    printf("<str,");
//    while(yytext[idx+1]){
//        printf("%c",yytext[idx]);
//        idx++;
//    }
//    printf(">");
//}

void print_error(){
    printf("Lexical error: '%s' in line number %d\n",yytext,yylineno);
    exit(1);
}