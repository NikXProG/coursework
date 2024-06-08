#include "../include/expression.h"

std::list<ExpressionHandler::Variable> ExpressionHandler::variables;
size_t ExpressionHandler::MAX_RAM;
size_t ExpressionHandler::OCCUPIED_RAM;

void ExpressionHandler::executeFromFile(const std::string& path, size_t maxram) {
    MAX_RAM = maxram;
    OCCUPIED_RAM = 0;
    std::ifstream inFile(path);
    if (!inFile.is_open()) {
        throw std::logic_error("Unable to open file: " + path);
    }
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    inFile.close();
    executeCommands(buffer.str());
}

void ExpressionHandler::executeCommands(const std::string& cmd) {
    std::string line;
    std::stringstream commandStream(cmd);
    std::stringstream nestedExpression;
    size_t expressionDepth = 0;
    int variableCount = 0;

    while (std::getline(commandStream, line)) {
        if (line.find('{') != std::string::npos) {
            ++expressionDepth;
            if (expressionDepth == 1) continue;
        }

        if (line.find('}') != std::string::npos) {
            --expressionDepth;
            if (expressionDepth == 0) {
                executeCommands(nestedExpression.str());
                nestedExpression.str("");
                continue;
            }
        }

        if (expressionDepth != 0) {
            nestedExpression << line << "\n";
        } else {
            if (line.find('=') != std::string::npos) {
                std::string name, cmd;
                parseAssignmentExpression(line, &name, &cmd);
                try {
                    executeFunction(cmd, name);
                } catch(const std::exception& e) {
                    throw;
                }
                ++variableCount;
                continue;
            }

            if (line.find("ShowVar") != std::string::npos) {
                try {
                    showVariables();
                } catch (const std::exception& e) {
                    throw;
                }
                continue;
            }
        }
    }
    checkAndDisposeVariables(variableCount);
}

void ExpressionHandler::checkAndDisposeVariables(size_t count) {
    size_t start = variables.size() - count;
    size_t i = 0;
    for (auto& var : variables) {
        if (i >= start) {
            var.canDispose = true;
        }
        ++i;
    }
}

void ExpressionHandler::parseAssignmentExpression(const std::string& line, std::string* name, std::string* calc) {
    std::stringstream ss;
    bool recorded = false;
    for (char c : line) {
        if (!recorded && c == '=') {
            *name = ss.str();
            ss.str("");
            recorded = true;
            continue;
        }
        if (c == ';') {
            break;
        }
        ss << c;
    }
    *calc = ss.str();
}

void ExpressionHandler::executeFunction(const std::string& cmd, const std::string& varname) {
    std::string funcName;
    std::list<std::string> args;
    parseFunctionExpression(cmd, &funcName, &args);

    if (funcName == "new") {
        size_t value = std::stoi(args.front());
        void* ptr = allocateMemory(value);
        addVariable(varname, ptr, value);
    }
}

void ExpressionHandler::parseFunctionExpression(const std::string& cmd, std::string* name, std::list<std::string>* variables) {
    std::stringstream ss;
    bool isNameRecorded = false;
    size_t depth = 0;
    for (char c : cmd) {
        if (c == ')') --depth;
        if (c == '(') {
            ++depth;
            if (!isNameRecorded) {
                isNameRecorded = true;
                *name = ss.str();
                ss.str("");
                continue;
            }
        }
        if (isNameRecorded && (c == ',' || (c == ')' && depth == 0))) {
            variables->push_back(ss.str());
            ss.str("");
            continue;
        }
        ss << c;
    }
}

void* ExpressionHandler::allocateMemory(size_t value) {
    if (OCCUPIED_RAM + value > MAX_RAM) {
        for (auto& var : variables) {
            if (var.canDispose && var.ptr != nullptr) {
                delete[] static_cast<char*>(var.ptr);
                var.ptr = nullptr;
                OCCUPIED_RAM -= var.size;
            }
        }
    }

    if (OCCUPIED_RAM + value <= MAX_RAM) {
        try {
            void* var = ::operator new(value);
            OCCUPIED_RAM += value;
            return var;
        } catch (const std::exception& e) {
            throw std::logic_error("Memory allocation failed: " + std::string(e.what()));
        }
    } else {
        throw std::logic_error("Unable to allocate memory: insufficient memory available");
    }
}

ExpressionHandler::Variable ExpressionHandler::findVariable(const std::string& searchName) {
    for (const auto& var : variables) {
        if (var.name == searchName) {
            return var;
        }
    }
    throw std::logic_error("Variable not found: " + searchName);
}

void ExpressionHandler::addVariable(const std::string& name, void* ptr, size_t size) {
    for (auto& var : variables) {
        if (var.name == name) {
            delete[] static_cast<char*>(var.ptr);
            var.ptr = ptr;
            var.canDispose = false;
            var.size = size;
            return;
        }
    }

    Variable newVar;
    newVar.ptr = ptr;
    newVar.name = name;
    newVar.canDispose = false;
    newVar.size = size;
    variables.push_back(newVar);
}

void ExpressionHandler::printVariables() {
    std::cout << "Variables: ";
    for (const auto& var : variables) {
        std::cout << var.name << ": " << var.canDispose << "; ";
    }
    std::cout << "\n";
}

void ExpressionHandler::showVariables() {
    std::cout << "Active Variables: ";
    for (const auto& var : variables) {
        if (!var.canDispose) {
            std::cout << var.name << " (" << var.size << " bytes); ";
        }
    }
    std::cout << "\n";
}

ExpressionHandler::~ExpressionHandler() {
    for (auto& var : variables) {
        delete[] static_cast<char*>(var.ptr);
    }
}
