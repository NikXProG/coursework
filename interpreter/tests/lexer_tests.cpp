#include <iostream>
#include <lexer_builder.h>

int main(){

    try{

        auto lex_builder = new lexer_builder();
        auto sosal = lex_builder->inizialize("../../stream.txt")->tokenization();
        for (int i = 0; i < sosal->tokens->size; i++){
            std::cout << lexer::token_to_string(sosal->tokens->token[i].type) << " " << sosal->tokens->token[i].value << std::endl;
        }

    
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


