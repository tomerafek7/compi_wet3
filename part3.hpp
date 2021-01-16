
#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <iostream>

using namespace std;

typedef enum
{
    INT,
    FLOAT,
    VOID
} Type;

typedef struct line {
int quad; // command number
string type; // Syntax variable or token type for tokens
string value; // Token value. NULL for syntax variables
int regnum;
vector<string> code;
vector<int> truelist;
vector<int> falselist;
vector<int> nextlist;

} Line;

class SemanticException: public exception{
public:
    int line_num;
    string error;
    SemanticException(int line_num, const char* error):
    line_num(line_num), error(error){}

    const char * what () const throw () {
        return "Semantic error: <error description> in line number <line_number>";
    }
};



class Commands {
private:
    vector<string> command_list;
public:
    Commands();

    void backpatch(vector<int> list, int address);

    vector<int> &merge(vector<int> list_1, vector<int> list_2);

    void emit(string command);

    int nextquad();

    vector<int> &makelist(int value);

};



class Function{

public:
    Function(int dec_line, Type return_type, vector<Type> & api);
    Function(int dec_line, Type return_type);
    void insertApi(vector<Type> & api);
    void insertScopes(vector<int> & scopes);
    int dec_line;
    vector<int>* calls;
    Type return_type;
    vector<Type>* api;
    vector<int>* scopes;

    void add_call(int line);

};


class FunctionTable{

public:

    FunctionTable();

    map<string, Function*>* table;

    void add_function(string &name, int dec_line, Type return_type);

    void add_api(string &name, vector<Type> & api);

    void add_scopes(string &name, vector<int> & scopes);

    void add_call(string &name, int call_line, vector<Type> & api, vector<int> & scopes);

};


class Symbol{

public:
    string name;
    int address;
    Type type;

    Symbol(string &name, int address, Type type);
};

class SymbolTable{

public:
    map<string,Symbol*> *table;

    SymbolTable() = default;

    void add_symbol(int call_line, string &name, int offset, Type type);
    int get_symbol_offset(int call_line, string &name);
    Type get_symbol_type(int call_line, string &name);

};


extern Commands *commands;

extern stack<SymbolTable>* symbol_table_stack;

extern int RegisterIdx[2];

extern SymbolTable* currnet_sym_tbl;

extern FunctionTable* function_table;

vector<int>* scopes_api;

int offset = -4;

void SemanticError(int line_num, const char* error){
    cerr << "Semantic error: <error description> in line number <line_number>";
}




#define YYSTYPE Line*

#endif //COMMON_H
