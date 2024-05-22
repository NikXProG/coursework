#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
int main(){

    nlohmann::json objJson;

    std::ifstream jsonFile("./dictionary.json");

    if (!jsonFile.is_open()){
        return -1;
    }
    
    jsonFile >> objJson;

    jsonFile.close();

    std::cout << objJson << std::endl;
    for (auto &x : objJson.items())
    {
        std::cout << x.key() << std::endl << std::endl;

        for (auto &y: x.value().items()){
            std::cout << to_string(y.value()) << std::endl;
        }
        std::cout << std::endl;
    }

    
    return 0;
}