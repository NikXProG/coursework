#include <iostream>
#include "../lexer/include/lexer.h"
#include "../parser/include/parser.h"

int main() {
    try {

        Lexer lexer("../../stream.txt");

        lexer.readFile();

        lexer.lexAnalysis();

        Parser parser(lexer);

        ExpressionNode* rootNode = parser.parse();
        
        parser.run(rootNode);

        delete rootNode;

        return 0;

    } catch(const std::runtime_error& ex) {
        std::cout << ex.what() << std::endl;
        return -1; 
    } catch(const std::out_of_range& ex) {
        std::cout << ex.what();
        return -1;
    } catch(const std::exception& ex) {
        std::cout << ex.what();
        return -1;
    }
}
