
#ifndef COMMON_H
#define COMMON_H

#include <stdlib>
#include <string>
#include <vector>

using std::vector;


typedef struct line {
String type; // Syntax variable or token type for tokens
String value; // Token value. NULL for syntax variables
vector<String> code;
vector<int> truelist;
vector<int> falselist;
vector<int> nextlist;

} Line;


class Commands{
    
    Commands();
    
    vector<String> command_list;
    
    void backpatch(vector<int> list, int address);
    
    vector<int> merge(vector<int> list_1, vector<int> list_2);
    
    void emit(String command);
    
    int nextquad();
    
    vector<int> makelist(int value);
    
}












#define YYSTYPE Line*

#endif //COMMON_H
