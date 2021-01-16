#include "part3.hpp"



Commands::Commands(){}

void Commands::backpatch(vector<int> list, int address){
    for (vector<int>::iterator it = list.begin() ; it != list.end(); ++it){
        command_list[it] += to_string(address);
    }
}

vector<int> Commands::merge(vector<int> list_1, vector<int> list_2){
    vector<int>* res = new vector<int>();
    res->reserve( list_1.size() + list_2.size() ); // preallocate memory
    res->insert( res->end(), list_1.begin(), list_1.end() );
    res->insert( res->end(), list_2.begin(), list_2.end() );
    return res;
}

void Commands::emit(string command){
    this.command_list.push_back(command);
}

int Commands::nextquad(){
    return this.command_list.size();
}

vector<int>& Commands::makelist(int value){
    vector<int> *res = new vector<int>();
    res->push_back(value);
    return *res;
}
    

Function::Function(int dec, type return_type, vector<type> api) {
    dec = dec;
    return_type = return_type;
    api = api;



}





