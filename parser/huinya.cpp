#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <stdarg.h>
#include <malloc.h>
#include <cstring>
#include <stack>

void reading_file_in_buffer(const char* filepath, struct source_code* Source);
bool lexical_analysis_check(const char* command);
bool syntax_analysis_check(const char* command);
bool semantic_analysis_check(const char* command);
void parser(const char* command);
void tests(char**& buff);
bool key_char_check(const char* command);
int find_first_of(const char* str, char find_c);

//void create_stack(source_code*);

struct Token {
    size_t size = 0;
    const char* type = "Token";
    char** token_array = nullptr;
};

struct str_code {
    char* code = nullptr;
    bool flag_def;
};

struct source_code {
    size_t size = 0;
    const char* type = "Code";
    str_code* source_array = nullptr;
};


int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            throw "Error: invalid count of arguments";
        }

        // создание зарезервированных слов

        Token token;
        token.token_array = new char*[++token.size];
        token.token_array[0] = new char[strlen("print") + 1];
        strcpy(token.token_array[0], "print");


        char* file_path = argv[argc - 1];

        source_code Source;

        // запись в память из файла
        reading_file_in_buffer(file_path, &Source);

        for (int i = 0; i < Source.size; i++) {
            std::cout << Source.source_array[i].code << std::endl;
        }

        // различные степени проверки
        // create_stack(Source);

        // tests(buffer); 

        //free memory
        for (int i = 0; i < token.size; i++) {
            delete[] token.token_array[i];
        }
        delete[] token.token_array;

        return 0;

    } catch (const char* error_message)
    {
        std::cout << error_message;
    }

}

void reading_file_in_buffer(const char* filepath, struct source_code* Source) {

    std::ifstream inf(filepath);

    // проверка указателя на nullptr
    if (!inf.is_open()) {
        throw std::ios_base::failure("Error: opening/closing/reading file");
    }

    std::string line;
    int size = 0;

    // длина исходного кода
    while (getline(inf, line)) Source->size++;

    Source->source_array = new str_code[++Source->size];

    if (Source->source_array == nullptr) {
        throw std::bad_alloc();
    }

    int i = 0;

    // смещение указателя
    inf.clear();
    inf.seekg(0);

    // backup
    while (getline(inf, line)) {
        Source->source_array[i].code = new char[line.length() + 1];

        if (Source->source_array[i].code == nullptr) {
            throw std::bad_alloc();
        }

        strcpy(Source->source_array[i].code, line.c_str());
        i++;
    }

    inf.close();
}

int find_first_of(const char* str, char find_c) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == find_c) {
            return i;
        }
    }
    return -1;
}
 /*void create_stack( source_code& Source){
    for (int i = 0; i < Source.size; i++){
        //find_first_of(buffer[i], '(');
        

    }
}*/

/*
void tests(char **& buffer, Token tokens){
    size_t size_buffer = sizeof(buffer);

    for (int i=0;i<size_buffer; i++){
        std::cout << lexical_analysis_check(buffer[i], tokens);
        std::cout << syntax_analysis_check(buffer[i]);
        std::cout  << semantic_analysis_check(buffer[i]);
    }    
}

bool lexical_analysis_check(const char * command, Token tokens){

    bool flag_token_on = false;

    // базовая проверка на ; в конце
    if (!find_first_of(command, ';')){
        return false;
    }   
    // проверка на зарезервированные команды
    if (!KeyCommandCheck(command,  tokens,  flag_token_on )) return false;
    if (flag_token_on && find_first_of(command, ';')){ // там есть print и там есть ;
        // print;
        // print ccc;

    }  
}

// попробуем сразу же определить кто перед нами пользовательская функция, переменная или функция из под коробки.
bool KeyCharCheck(const char * command){
    if (!find_first_of(command, ';')){
        return false;
    }
    return true;
}

bool KeyCommandCheck(const char * command, Token tokens, bool flag_token){
    for (int i = 0; i < tokens.size ; i++){
        if( strstr (command, tokens.token_array[i]) != NULL){
            flag_token = true;
        } 
    }
    return true;
}*/




        // далее


        /*auto add = [](auto x, auto y) {return ((x*y+2)*(25-x/y))/(3-(x+2*y));};
        auto add1 = [](auto z) {return (z*z+4);};
        int myvar = 15;
        float bg = 25;
        double ccc =  add1(bg+myvar)*15+add(bg*25, (6*add1(myvar-10)));
        cout << ccc;    for (int i = 0; i < size_buffer; i++) {

        if (buffer[i]){

        }




        for (char *p = buffer[i]; *p != '\0'; p++){
            if (*p == ':'){
                cout << "Это функция";
                
            }
            else if(*p=='='){
                cout << "Это переменная ";
            }
            else{
                cout << "Это не функция и не переменная ( скорее всего команда)";
            }
        }

    }*/