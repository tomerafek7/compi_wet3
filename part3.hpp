
#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <string>
#include <vector>
#include <map>

using namespace std;


typedef struct line {
int quad; // command number
string type; // Syntax variable or token type for tokens
string value; // Token value. NULL for syntax variables
vector<string> code;
vector<int> truelist;
vector<int> falselist;
vector<int> nextlist;

} Line;

typedef enum
{
    INT,
    FLOAT,
    VOID
} Type;

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

extern Commands *commands;

class Function{

public:
    Function(int dec_line, Type return_type, vector<Type> & api);

    int dec_line;
    vector<int>* calls;
    Type return_type;
    vector<Type>* api;

    void add_call(int line);

};


class FunctionTable{

public:

    FunctionTable();

    map<string, Function*>* table;

    void add_function(string &name, int dec_line, Type return_type, vector<Type> & api);

    void add_call(string &name, int call_line, vector<Type> & api);

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

    void add_symbol(int call_line, string &name, int address, Type type);
    int get_symbol_address(int call_line, string &name);
    Type get_symbol_type(int call_line, string &name);

};









#define YYSTYPE Line*

#endif //COMMON_H
