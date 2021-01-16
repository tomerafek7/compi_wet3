#include "part3.hpp"

#include <utility>



Commands::Commands(){}

Commands *commands = new Commands();

FunctionTable* function_table = new FunctionTable();

stack<SymbolTable>* symbol_table_stack = new stack<SymbolTable>();

SymbolTable* currnet_sym_tbl = new SymbolTable();



void Commands::backpatch(vector<int> list, int address){
    for (vector<int>::iterator it = list.begin() ; it != list.end(); ++it){
         command_list[*it] += to_string(address);
    }
}

vector<int>& Commands::merge(vector<int> list_1, vector<int> list_2){
    vector<int>* res = new vector<int>();
    res->reserve( list_1.size() + list_2.size() ); // preallocate memory
    res->insert( res->end(), list_1.begin(), list_1.end() );
    res->insert( res->end(), list_2.begin(), list_2.end() );
    return *res;
}

void Commands::emit(string command){
    command_list.push_back(command);
}

int Commands::nextquad(){
    return command_list.size();
}

vector<int>& Commands::makelist(int value){
    vector<int> *res = new vector<int>();
    res->push_back(value);
    return *res;
}
    

Function::Function(int dec_line, Type return_type, vector<Type> & api):
        dec_line(dec_line), return_type(return_type)
{
    this->api = new vector<Type>(api);
    this->calls = new vector<int>();
}

Function::Function(int dec_line, Type return_type):
        dec_line(dec_line), return_type(return_type)
{
    this->api = new vector<Type>();
    this->calls = new vector<int>();
}

void Function::insertApi(vector<Type> & api){
    *this->api = api;
}

void Function::insertScopes(vector<int> & scopes){
    *this->scopes = scopes;
}

void Function::add_call(int line) {
    this->calls->push_back(line);
}

void FunctionTable::add_function(string &name, int dec_line, Type return_type){
    std::pair<std::map<string,Function*>::iterator,bool> res;
    res = table->insert(std::pair<string, Function*>(name,new Function(dec_line, return_type)));
    if (!res.second) { // there's already a function with this name
        throw SemanticException(dec_line, "Redeclaration of Function");
    }
}
void FunctionTable::add_api(string &name, vector<Type> & api){
    table->at(name)->insertApi(api);
}

void FunctionTable::add_scopes(string &name, vector<int> & scopes) {
    table->at(name)->insertScopes(scopes);
}

void FunctionTable::add_call(string &name, int call_line, vector<Type> & api){
    // firstly, check that this function really declared
    if (!table->count(name)){
        throw SemanticException(call_line, "Function is not declared");
    }
    // secondly, check that
}

Symbol::Symbol(string &name, int address, Type type):
name(name), address(address), type(type){}


void SymbolTable::add_symbol(int call_line, string &name, int offset, Type type){
    std::pair<std::map<string,Symbol*>::iterator,bool> res;
    res = table->insert(std::pair<string, Symbol*>(name,new Symbol(name, offset, type)));
    if (!res.second) { // there's already a function with this name
        SemanticError(call_line, "Redeclaration of Variable");
    }

}
int SymbolTable::get_symbol_offset(int call_line, string &name){

}
Type SymbolTable::get_symbol_type(int call_line, string &name){

}




