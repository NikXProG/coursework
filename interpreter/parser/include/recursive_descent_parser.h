#include <iostream>
#include <vector>
#include <string>

class RecursiveDescentParser {

    std::vector<std::string> tokens;
    int pos = 0; // индекс текущего токена    



public:

    RecursiveDescentParser(std::vector<std::string> tokens) {
        this->tokens = tokens;
    }

    double parse(){
        double result = expression();

        if (pos != tokens.size()) {
            throw std::runtime_error("error in expression token: " + tokens[pos]);
        }
        return result;    
    }

private:

    double expression(){

        // находим первое слагаемое
        double first = term();
        while (pos < tokens.size()) {
            std::string op = tokens[pos];
            if (op.compare("+") && op.compare("-")) {
                break;
            }
            pos++;
            // находим второе слагаемое (вычитаемое)
            double second = term();
            if (!op.compare("+")) {
                first += second;
            } else {
                first -= second;
            }
        }
        return first;
    }
    double term(){

            // находим первый множитель
            double first = factor();
            while (pos < tokens.size()) {
                std::string op = tokens[pos];

                if (op.compare("*") && op.compare("/")) {
                    break;
                }
                pos++;
                // находим второй множитель (делитель)
                double second = factor();

                if (!op.compare("*")) {
                    first *= second;
                } else {
                    first /= second;
                }
            }
            return first;    
    }
    double factor(){

        std::string next = tokens[pos];
        if (next == "(") {
            pos++;
            // если выражение в скобках, то рекурсивно переходим на обработку подвыражения типа E
            double result = expression();
            if (pos < tokens.size() && tokens[pos] == ")") {
                pos++;
                return result;
            } else {
                throw std::runtime_error("waiting ')', but find is " + tokens[pos]);
            }
        } else {
            pos++;
            return std::stod(next);
        }    
    }
};