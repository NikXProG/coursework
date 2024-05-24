#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <ctime>

#include <employee.h>

void readDataFromFile(std::string filename, std::map<std::string, Employee>& employees);

int main() 
{
    try
    {
        Employee employee;
        std::map<std::string, Employee> employees;
        readDataFromFile("../../file.txt", employees);
        
        employee.calculateTotalCost("Sidorov Ivan Ivanovich", employees);
        employee.listContracts("Kozlov Sergey Petrovich", employees);
        employee.FindMostExpensiveContract("Smirnova Anna Ivanovna", employees);
        employee.FindLongestContract("Popova Irina Sergeyevna", employees);

    }
    catch(const std::invalid_argument& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}

void readDataFromFile(std::string filename, std::map<std::string, Employee>& employees) 
{
    std::ifstream file(filename);

    if (!file.is_open()) 
    {
        throw std::ifstream::failure("File couldn't be opened");
    } 
    else 
    {
        std::string line;
        std::string prev_line;
        while (std::getline(file, line)) 
        {
            if (line.empty()) 
                continue;

            if (line.find('{') != std::string::npos) 
            {
                Employee emp;
                emp.fullName = prev_line;
                while (std::getline(file, line)) 
                {
                    if (line.find('}') != std::string::npos) 
                        break;
                    
                    Contract contract;
                    std::string trash;
                    std::istringstream iss(line);
                    iss >> trash>> contract.number >> trash >>  contract.startDate >> trash>> contract.endDate >> trash >> trash >>trash>> contract.cost;
                    emp.contracts.push_back(contract);
                }
                employees[emp.fullName] = emp;
            }
            prev_line = line;
        }
        file.close();
    }
}
