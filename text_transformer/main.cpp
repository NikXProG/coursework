#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

int main(int argc, char * argv[] ){

    if (argc == 0){
        return 1;
    }

    nlohmann::json objJson;

    std::ifstream fileInput("C:/coursework/text_transformer/assets/file.txt");
    if (!fileInput.is_open()){
        return -1;
    }

    std::ifstream jsonFile("../dictionary.json");

    if (!jsonFile.is_open()){
        return -2;
    }

    jsonFile >> objJson;

    jsonFile.close();

    std::string line;

    while(getline(fileInput,line)){
        auto arrayKeyValue = objJson.items();    
        for (auto &x : arrayKeyValue)
        {   
            auto arrayKeySynoword = x.value().items();
            for (auto &y: arrayKeySynoword){
                std::string syno = y.value().get<std::string>();
                int index = line.find(syno);
                if (index != std::string::npos) {
                    std::cout << "replace string " << line.substr(index, syno.size() + 1) << " to " << x.key() << std::endl;
                    std::cout << line << std::endl << std::endl << std::endl;
                    line.replace(index, syno.size(), x.key());
                   
                    std::cout << line << std::endl << std::endl << std::endl;

                }
            }
        }
    }


    return 0;
}