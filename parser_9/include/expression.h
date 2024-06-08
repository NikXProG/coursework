#include <sstream>
#include <iostream>
#include <fstream>
#include <list>
#include <stdexcept>

class ExpressionHandler {
private:
    static size_t MAX_RAM;
    static size_t OCCUPIED_RAM;

    struct Variable {
        std::string name;
        void* ptr = nullptr;
        bool canDispose = false;
        size_t size = 0;
    };

    static std::list<Variable> variables;

    ~ExpressionHandler();

    static Variable findVariable(const std::string& searchName);
    static void addVariable(const std::string& name, void* ptr, size_t size);

    static void executeFunction(const std::string& cmd, const std::string& varname);
    static void parseAssignmentExpression(const std::string& line, std::string* name, std::string* calc);
    static void parseFunctionExpression(const std::string& cmd, std::string* name, std::list<std::string>* variables);
    static void showVariables();
    static void* allocateMemory(size_t value);

    static void checkAndDisposeVariables(size_t count);

public:
    static void executeFromFile(const std::string& path, size_t maxram);
    static void executeCommands(const std::string& cmd);
    static void printVariables();
};