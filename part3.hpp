
#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <string>
#include <vector>

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


class Commands{
    
    Commands();
    
    vector<string> command_list;
    
    void backpatch(vector<int> list, int address);
    
    vector<int> merge(vector<int> list_1, vector<int> list_2);
    
    void emit(string command);
    
    int nextquad();
    
    vector<int> makelist(int value);
    
}












#define YYSTYPE Line*

#endif //COMMON_H
