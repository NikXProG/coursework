#include <iostream>
#include <lexer_builder.h>
#include <parser_builder.h>
int main(){

    try{

        auto lex_builder = new lexer_builder();
        auto lexer = lex_builder->inizialize("../../stream.txt")->tokenization();
        auto parser = new parser_builder()->BuildAST(lexer);       
    
    }

    catch(const std::runtime_error& ex){
        std::cout << ex.what() << std::endl;
        return -1; 
    }
    catch (const std::out_of_range& ex) {
       std::cout << ex.what();
    }
    catch (const std::exception& ex) {
       std::cout << ex.what();
    }

}


