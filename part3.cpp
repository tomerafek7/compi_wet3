#include "part3.hpp"

#include <utility>

extern "C"	FILE *yyin;

// global variables
Commands* commands = new Commands();
stack<SymbolTable*>* symbol_table_stack = new stack<SymbolTable*>();
int RegisterIdx[2];
SymbolTable* current_sym_tbl = new SymbolTable();
FunctionTable* function_table = new FunctionTable();
vector<int>::iterator scopes_iter;
vector<int>* scopes_api;
vector<Symbol*>* args_api;
vector<int>* called_scopes;
vector<Symbol*>* called_args;
int offset;
bool in_scope = false;


Commands::Commands(){
    command_list = new vector<string>();
}

void Commands::backpatch(vector<int>& list, int address){
    for (vector<int>::iterator it = list.begin() ; it != list.end(); ++it){
         command_list->at(*it) += to_string(address);
    }
}

vector<int>& Commands::merge(vector<int>& list_1, vector<int>& list_2){
    vector<int>* res = new vector<int>();
    res->reserve( list_1.size() + list_2.size() ); // preallocate memory
    res->insert( res->end(), list_1.begin(), list_1.end() );
    res->insert( res->end(), list_2.begin(), list_2.end() );
    return *res;
}

void Commands::emit(string command){
    command_list->push_back(command);
    cout << command << endl;
}

int Commands::nextquad(){
    return command_list->size();
}

vector<int>& Commands::makelist(int value){
    vector<int> *res = new vector<int>();
    res->push_back(value);
    return *res;
}

Function::Function(int dec_line, Type return_type, vector<Symbol*> & api,
                    vector<int> & scopes, string name) : dec_line(dec_line), return_type(return_type), name(name)
{
    this->api = new vector<Symbol*>(api);
    this->scopes = new vector<int>(scopes);
    this->calls = new vector<int>();
}


Function::Function(Function *f){
    this->dec_line=f->dec_line;
    this->return_type = f->return_type;
    this->name = f->name;
    this->api= new vector<Symbol*>(*f->api);
    this->scopes = new vector<int>(*f->scopes);
    this->calls = new vector<int>(*f->calls);
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

FunctionTable::FunctionTable(){
    table = new map<string, Function*>();
}

int FunctionTable::get_dec_line(string& name) {
    return this->table->at(name)->dec_line;
}
vector<Symbol*>* FunctionTable::get_api(string& name){
    return new vector<Symbol*>(*this->table->at(name)->api);
}

vector<int>* FunctionTable::get_scope(string &name){
    return new vector<int>(*this->table->at(name)->scopes);
}

// dec_line = -1 if this is only a declaration.
void FunctionTable::add_function(string &name, int dec_line, Type return_type,
        vector<Symbol*>* api, vector<int>* scopes){
    std::pair<std::map<string,Function*>::iterator,bool> res;
    res = table->insert(std::pair<string, Function*>(name,
                           new Function(dec_line, return_type, *api, *scopes, name)));
    // if the insert didn't succeeded && this call is for declaration --> ERROR
    if (!res.second && dec_line != -1) { // there's already a function with this name
        // check if this function is only declared / already implemented
        if(res.first->second->dec_line == -1){ // only declared
            // check if api & scopes are similar:
            if(*api != *res.first->second->api || *scopes != *res.first->second->scopes){
                SemanticError(dec_line, "Wrong API/Scopes for implementation of function");
                // assuming SemanticError calls exit()
            }
            // if they are:
            // 1. update the dec_line
            table->at(name)->dec_line = dec_line;
            // 2. update the dec_line for all calls:
            for(auto it = table->at(name)->calls->begin(); it != scopes->end() ; ++it){
                commands->command_list->at(*it) = "JLINK " + to_string(dec_line);
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

void FunctionTable::add_call(string &name, int call_line, vector<Symbol*>* api, vector<int>* scopes){
    // firstly, check that this function really declared
    if (!table->count(name)){
        SemanticError(call_line, "Function is not declared");
    }
    if(this->table->at(name)->api->size() != api->size()){
        SemanticError(call_line, "Function Symboluments: size mismatch");//FIXME
    }
    Function* curr_func  = this->table->at(name);
    vector<Symbol*> *called_func_api = curr_func->api;
    for(vector<Symbol*>::iterator it = api->begin(), iter = called_func_api->begin(); it != api->end() || iter != called_func_api->end() ; ++it){
        if((*it)->type != (*iter)->type){
            SemanticError(call_line, "Function Symboluments type mismatch");
        }
        ++iter;
    }
    vector<int> *called_func_scopes = curr_func->scopes;
    for(vector<int>::iterator it = scopes->begin(); it != scopes->end() ; ++it){
        if(find(called_func_scopes->begin(), called_func_scopes->end(), *it) != called_func_scopes->end()){
            SemanticError(call_line,  "Function scope error" );//FIXME
        }
    }
    curr_func->add_call(call_line);
}

vector<Function>* FunctionTable::get_all_implemented(){
    vector<Function>* out = new vector<Function>();
    for(map<string, Function*>::iterator it = this->table->begin();it != table->end();++it){
        if(it->second->dec_line!=-1){
            out->push_back(it->second);
        }
    }
    return out;
}

vector<Function>* FunctionTable::get_all_unimplemented(){
    vector<Function>* out = new vector<Function>();
    for(map<string, Function*>::iterator it = this->table->begin();it != table->end();++it){
        if(it->second->dec_line==-1){
            out->push_back(it->second);
        }
    }
    return out;
}



Symbol::Symbol(int offset, Type type, string &name)  : offset(offset), type(type), name(name){}

Symbol::Symbol(int offset, Type type,  int reg, string &name) : offset(offset), type(type), reg(reg), name(name){}

Symbol::Symbol(Type type, string &name) :  type(type), name(name){}

Symbol::Symbol(Type type, int reg) :  type(type), reg(reg){}

SymbolTable::SymbolTable(){
    table = new map<string,Symbol*>();
}

void SymbolTable::add_symbol(int call_line, string &name, int offset, Type type){
    std::pair<std::map<string,Symbol*>::iterator,bool> res;
    res = table->insert(std::make_pair(name,new Symbol(offset, type, name)));
    if (!res.second) { // there's already a variable with this name
        SemanticError(call_line, "Redeclaration of Variable");
    }

}
int SymbolTable::get_symbol_offset(int call_line, string &name){

    if (!table->count(name)){
        SemanticError(call_line, "Function is not declared");
    }
    return this->table->at(name)->offset;
}
Type SymbolTable::get_symbol_type(int call_line, string &name){
    if (!table->count(name)){
        SemanticError(call_line, "Function is not declared");
    }
    return this->table->at(name)->type;
}
//
//Line* makeLine(const char *type, const char *value) {
//
//    Line *p;
//
//    if ((p = (Line *) (malloc(sizeof(Line)))) == 0)
//        fprintf(stderr, "Failed malloc(struct node)\n");
//    else {
//        p->type = INT;
//        if (value != nullptr) {
//            p->value = strdup(value);
//        } else {
//            p->value = string();
//        }
//    }
//    p->reg = 0;
//    p->quad = 0;
//    p->truelist = new vector<int>();
//    p->falselist = new vector<int>();
//    p->nextlist = new vector<int>();
//    return (p);
//}

void SemanticError(int line_num, const char* error){
    cerr << "Semantic error: <error description> in line number <line_number>";
    exit(Semantic);

}


/**************************************************************************/
/*                           Main of parser                               */
/**************************************************************************/
extern int yyparse (void);

//int main(){
//    string mystr = "abc";
////    SymbolTable* sym_tbl = new SymbolTable();
//    current_sym_tbl->add_symbol(30,mystr,1,INT);
//    map<string,Symbol> my_map = map<string,Symbol>();
////    auto res = my_map.insert(make_pair(mystr,Symbol(1,INT,mystr)));
////    printf("%d\n", (int)res.second);
////    printf("%d",(int)my_map.count(mystr));
//
//
//}

int main(int argc, char* argv[]){
    int rc;
    string arg_file = argv[1];
    string filename = arg_file.substr(0,arg_file.find_last_of('.'))+".rsk";

    ofstream file;
    file.open(filename);
    if(!file.is_open()) {
        cerr << "Operational error: cannot open input file" << endl;
        exit(Operational);
    }

//    streambuf* oldCStreamBuf = cin.rdbuf();
//    ostringstream strCout;
//    cin.rdbuf( strCout.rdbuf() );

#if YYDEBUG
    yydebug=1;
#endif

    yyin = fopen(argv[1], "r");
    rc = yyparse();
    assert (rc == 0);  // Parsed successfully
    // print header:
    file << "<header>" << endl;
    file << "<unimplemented>";
    vector<Function>* unimplemented = function_table->get_all_unimplemented();
    for(auto it = unimplemented->begin(); it != unimplemented->end(); ++it){
        file << it->name << ",[";
        for(auto s_it = it->scopes->begin(); s_it != it->scopes->end(); ++s_it){
            file << to_string(*s_it) << ";";
        }
        file << "],";
        for(auto c_it = it->scopes->begin(); c_it != it->scopes->end(); ++c_it){
            file << to_string(*c_it) << ",";
        }
        file << " ";
    }
    file << endl;
    file << "<implemented>" << endl;
    vector<Function>* implemented = function_table->get_all_implemented();
    for(auto it = implemented->begin(); it != implemented->end(); ++it){
        file << it->name << ",[";
        for(auto s_it = it->scopes->begin(); s_it != it->scopes->end(); ++s_it){
            file << to_string(*s_it) << ";";
        }
        file << "],";
        for(auto c_it = it->scopes->begin(); c_it != it->scopes->end(); ++c_it){
            file << to_string(*c_it) << ",";
        }
        file << " ";
    }
    file << endl;
    file << "</header>" << endl;

    // print assembly:
    for(auto it = commands->command_list->begin(); it != commands->command_list->end(); ++it){
        file << *it << endl;
    }

    file.close();
    return rc;
}
