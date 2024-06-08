
#include <iostream>
#include <Expression.h>

int main()
{
    try
    {
        ExpressionHandler::executeFromFile("../../stream.txt", 1024);
        
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
    return 0;
}