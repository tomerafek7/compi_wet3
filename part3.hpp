
#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <algorithm>


#define INIT_REG_INT 3
#define INIT_REG_FLOAT 0
using namespace std;

typedef enum
{
    INT,
    FLOAT,
    VOID
} Type;

typedef enum
{
    Lexical = 1,
    Syntax = 2,
    Semantic = 3,
    Operational = 9
} ERROR;

typedef struct line {
int quad; // command number
Type type; // Syntax variable or token type for tokens
string value; // Token value. NULL for syntax variables
int reg;
vector<int>* truelist;
vector<int>* falselist;
vector<int>* nextlist;

} Line;

Line* makeLine(const char *type, const char *value);


class Commands {
public:
    vector<string> command_list;

    Commands();

    void backpatch(vector<int>* list, int address);

    vector<int>* merge(vector<int>* list_1, vector<int>* list_2);

    void emit(string command);

    int nextquad();

    vector<int>* makelist(int value);

};

class Symbol{

public:

    int offset;
    Type type;
    int reg;
    string name;

    Symbol(int offset, Type type, string &name);
    Symbol(int offset, Type type, int reg , string &name);
    Symbol(Type type, string &name);
    Symbol(Type type, int reg);
    inline bool operator==(const Symbol& sym){
        return type == sym.type && name == sym.name;
    }
    inline bool operator!=(const Symbol& sym){
        return type != sym.type && name != sym.name;
    }
};


class SymbolTable{

public:
    map<string,Symbol*> *table;

    SymbolTable() = default;

    void add_symbol(int call_line, string &name, int offset, Type type);
    int get_symbol_offset(int call_line, string &name);
    Type get_symbol_type(int call_line, string &name);

};


class Function{

public:
    Function(int dec_line, Type return_type, vector<Symbol> & api, vector<int> & scopes, string name);
    Function(Function *f);
    // dec_line = -1 if this is only a declaration.
//    Function(int dec_line, Type return_type);
//    void insertApi(vector<Type> & api);
//    void insertScopes(vector<int> & scopes);
    int dec_line;
    vector<int>* calls;
    Type return_type;
    vector<Symbol>* api;
    vector<int>* scopes;
    string name;
    void add_call(int line);

};


class FunctionTable{

public:

    FunctionTable();

    map<string, Function*>* table;

    void add_function(string &name, int dec_line, Type return_type, vector<Symbol>* api, vector<int>* scopes);

    void add_api(string &name, vector<Type> & api);

    void add_scopes(string &name, vector<int> & scopes);

    vector<Symbol>* get_api(string& name);

    vector<int>* get_scope(string &name);

    void add_call(string &name, int call_line, vector<Symbol>* api, vector<int>* scopes);

    int get_dec_line(string& name);

    vector<Function>* get_all_implemented();

    vector<Function>* get_all_unimplemented();

};



extern Commands *commands;

extern stack<SymbolTable>* symbol_table_stack;

extern int RegisterIdx[2];

extern stack<int[2]>* reg_idx_stack;

extern SymbolTable* current_sym_tbl;

extern FunctionTable* function_table;

extern stack<int>* rtrn_vl_ofst_stk;

extern vector<int>::iterator scopes_iter;

vector<int>* scopes_api;
vector<Symbol>* args_api;
vector<int>* called_scopes;
vector<Symbol>* called_args;
int offset = -4;
bool in_scope = false;


void SemanticError(int line_num, const char* error){
    cerr << "Semantic error: <error description> in line number <line_number>";
    exit(Semantic);

}




#define YYSTYPE Line*

#endif //COMMON_H
