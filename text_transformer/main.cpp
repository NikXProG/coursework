#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <exception>

void loadSynonyms(const std::string& filepath, std::map<std::string, std::string>& synonyms) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::ios_base::failure("Error: unable to open synonyms file.");
    }

    nlohmann::json jsonSynonyms;
    file >> jsonSynonyms;
    file.close();

    for (auto& x : jsonSynonyms.items()) {
        for (auto& y : x.value()) {
            synonyms[y.get<std::string>()] = x.key();
        }
    }
}

void saveSynonyms(const std::string& filepath, const std::map<std::string, std::string>& synonyms) {
    nlohmann::json jsonSynonyms;

    for (const auto& pair : synonyms) {
        jsonSynonyms[pair.second].push_back(pair.first);
    }

    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::ios_base::failure("Error: unable to open synonyms file for writing.");
    }
    file << jsonSynonyms.dump(4);
    file.close();
}

void processFile(const std::string& inputFilePath, const std::string& outputFilePath, std::map<std::string, std::string>& synonyms, bool trainingMode, std::vector<std::pair<std::string, std::string>>& actions) {
    std::ifstream fileInput(inputFilePath);
    if (!fileInput.is_open()) {
        throw std::ios_base::failure("Error: unable to open input file.");
    }

    std::ofstream fileOutput(outputFilePath);
    if (!fileOutput.is_open()) {
        throw std::ios_base::failure("Error: unable to open output file.");
    }

    std::string word;
    while (fileInput >> word) {
        std::string originalWord = word;

        // Удаление знаков пунктуации
        if (ispunct(word.back())) {
            word.pop_back();
        }

        if (synonyms.find(word) != synonyms.end()) {
            std::string newWord = synonyms[word];
            fileOutput << newWord << " ";
            actions.emplace_back(originalWord, newWord);
        } else {
            if (trainingMode) {
                std::cout << word << " is not in the synonyms dictionary." << std::endl;
                std::cout << "Would you like to add it to the dictionary? (Y) or press enter to skip: ";
                char response;
                std::cin.get(response);
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (response == 'Y' || response == 'y') {
                    std::cout << "Enter the standard word for " << word << ": ";
                    std::string standardWord;
                    std::cin >> standardWord;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    synonyms[word] = standardWord;
                    fileOutput << standardWord << " ";
                    actions.emplace_back(originalWord, standardWord);
                } else {
                    fileOutput << originalWord << " ";
                }
            } else {
                fileOutput << originalWord << " ";
                actions.emplace_back(originalWord, "");
            }
        }
    }

    fileInput.close();
    fileOutput.close();
}

void undoActions(std::vector<std::pair<std::string, std::string>>& actions, int undoCount) {
    while (undoCount > 0 && !actions.empty()) {
        actions.pop_back();
        --undoCount;
    }
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 3) {
            throw std::invalid_argument("Error: invalid number of arguments! Correct structure: <synonyms_file> <text_file1> [text_file2 ...] [-train] [-undo N]");
        }

        bool trainingMode = false;
        int undoCount = 0;
        std::string synonymsFile = argv[1];
        std::vector<std::string> textFiles;

        for (int i = 2; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "-train") {
                trainingMode = true;
            } else if (arg == "-undo" && i + 1 < argc) {
                undoCount = std::stoi(argv[++i]);
            } else {
                textFiles.push_back(arg);
            }
        }

        std::map<std::string, std::string> synonyms;
        loadSynonyms(synonymsFile, synonyms);

        std::vector<std::pair<std::string, std::string>> actions;

        for (const auto& textFile : textFiles) {
            std::string outputFilePath = textFile + ".out";
            processFile(textFile, outputFilePath, synonyms, trainingMode, actions);
        }

        if (undoCount > 0) {
            undoActions(actions, undoCount);
        }

        // Сохранение словаря синонимов
        saveSynonyms(synonymsFile, synonyms);

        // Вывод замененных слов
        for (const auto& action : actions) {
            std::cout << action.first << " -> " << action.second << std::endl;
        }

    } catch (const std::ios_base::failure& ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    } catch (const std::invalid_argument& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 2;
    }

    return 0;
}