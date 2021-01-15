#include "part3.hpp"

class Commands{
    
    Commands(){
        this.command_list = new vector<String>();
    }
    
    void backpatch(vector<int> list, int address){
        for (vector<int>::iterator it = list.begin() ; it != list.end(); ++it){
            command_list[it] += to_string(address) 
        }
    }
    
    vector<int> merge(vector<int> list_1, vector<int> list_2){
        res = new vector<int>();
        res.reserve( list_1.size() + list_2.size() ); // preallocate memory
        res.insert( res.end(), list_1.begin(), list_1.end() );
        res.insert( res.end(), list_2.begin(), list_2.end() );
        return res;
    }
    
    void emit(String command){
        this.command_list.push_back(command);
    }
    
    int nextquad(){
        return this.command_list.size();
    }
    
    vector<int> makelist(int value){
        res = new vector<int>();
        res.push_back(value);
        return res;
    }
    
}





