#include "part3.hpp"

#include <utility>



Commands::Commands(){}

//FIXME move global variables to main

Commands *commands = new Commands();

FunctionTable* function_table = new FunctionTable();

stack<SymbolTable>* symbol_table_stack = new stack<SymbolTable>();

SymbolTable* currnet_sym_tbl = new SymbolTable();

stack<int[2]>* reg_idx_stack = new stack<int[2]>();

stack<int>* rtrn_vl_ofst_stk = new stack<int>();

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
    

Function::Function(int dec_line, Type return_type, vector<Arg> & api,
                    vector<int> & scopes):
        dec_line(dec_line), return_type(return_type)
{
    this->api = new vector<Arg>(api);
    this->scopes = new vector<int>(scopes);
    this->calls = new vector<int>();
}

//Function::Function(int dec_line, Type return_type):
//        dec_line(dec_line), return_type(return_type)
//{
//    this->api = new vector<Type>();
//    this->calls = new vector<int>();
//}

//void Function::insertApi(vector<Type> & api){
//    *this->api = api;
//}
//
//void Function::insertScopes(vector<int> & scopes){
//    *this->scopes = scopes;
//}

void Function::add_call(int line) {
    this->calls->push_back(line);
}

// dec_line = -1 if this is only a declaration.
void FunctionTable::add_function(string &name, int dec_line, Type return_type,
        vector<Arg> & api, vector<int> & scopes){
    std::pair<std::map<string,Function*>::iterator,bool> res;
    res = table->insert(std::pair<string, Function*>(name,
                           new Function(dec_line, return_type, api, scopes)));
    if (!res.second && dec_line != -1) { // there's already a function with this name
        // check if this function is only declared / already implemented
        if(res.first->second->dec_line == -1){ // only declared
            // check if api & scopes are similar:
            if(api != *res.first->second->api || scopes != *res.first->second->scopes){
                SemanticError(dec_line, "Wrong API/Scopes for implementation of function");
                // assuming SemanticError calls exit()
            }
            // if they are:
            // 1. update the dec_line
            table->at(name)->dec_line = dec_line;
            // 2. update the dec_line for all calls:
            for(auto it = table->at(name)->calls->begin(); it != scopes.end() ; ++it){
                commands->command_list[*it] = "JLINK " + to_string(dec_line);
            }
        } else{ // already implemented
            SemanticError(dec_line, "ReImplementation of Function");
        }
    } else{ // 2nd declaration
        SemanticError(dec_line, "Redeclaration of Function");
    }
}

//void FunctionTable::add_api(string &name, vector<Type> & api){
//    table->at(name)->insertApi(api);
//}
//
//void FunctionTable::add_scopes(string &name, vector<int> & scopes) {
//    table->at(name)->insertScopes(scopes);
//}

vector<int>* FunctionTable::add_call(string &name, int call_line, vector<Type> & api, vector<int> & scopes){
    // firstly, check that this function really declared
    if (!table->count(name)){
        SemanticError(call_line, "Function is not declared");
    }
    if(this->table->at(name)->api->size() != api.size()){
        if (!table->count(name)){
            SemanticError(call_line, "Function arguments: size mismatch");//FIXME
        }
    }
    Function* curr_func  = this->table->at(name);
    vector<Type> *called_func_api = curr_func->api;
    for(vector<Type>::iterator it = api.begin(), iter = called_func_api->begin(); it != api.end() || iter != called_func_api->end() ; ++it){
        if(*it != *iter){
            if (!table->count(name)){
                SemanticError(call_line, "Function arguments type mismatch");//FIXME
            }
        }
        ++iter;
    }
    vector<int> *called_func_scopes = curr_func->scopes;
    for(vector<int>::iterator it = scopes.begin(); it != scopes.end() ; ++it){
        if(find(called_func_scopes->begin(), called_func_scopes->end(), *it) != called_func_scopes->end()){
            if (!table->count(name)){
                SemanticError(call_line,  "Function scope error" );//FIXME
            }
        }
    }
    curr_func->add_call(call_line);
    return (new vector<int>(*curr_func->scopes));
}


Symbol::Symbol(int offset, Type type, string &name)  : offset(offset), type(type), name(name){}

Symbol::Symbol(int offset, Type type,  int reg, string &name) : offset(offset), type(type), reg(reg), name(name){}

Symbol::Symbol(Type type, string &name) :  type(type), name(name){}

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




