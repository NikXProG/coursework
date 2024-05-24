#include "../include/employee.h"

void Employee::calculateTotalCost(const std::string& fullName, std::map<std::string, Employee>& employees) 
{
    if (fullName.empty() || employees.empty() || employees.find(fullName) == employees.end())
    {
        throw std::invalid_argument("invalid argument detected in calculateTotalCost() functon!");
    }
    else
    {
        int totalCost = 0;
        for (const auto& contract : employees[fullName].contracts) 
        {
            totalCost += contract.cost;
        }
        std::cout << "Total cost for " << fullName << ": " << totalCost << std::endl<< std::endl;
    }
}

void Employee::listContracts(const std::string& fullName, std::map<std::string, Employee>& employees) 
{
    if (fullName.empty() || employees.empty()|| employees.find(fullName) == employees.end())
    {
        throw std::invalid_argument("invalid argument detected in listContracts() functon!");
    }
    else
    {
        std::cout << "Contracts for " << fullName << ":" << std::endl;
        for (const auto& contract : employees[fullName].contracts) 
        {
            std::cout << "Contract #" << contract.number << " - Cost: " << contract.cost << std::endl;
        }
        std::cout << std::endl;
    }
}

void Employee::FindLongestContract(const std::string& fullName, std::map<std::string, Employee>& employees) 
{
    if (fullName.empty() || employees.empty()|| employees.find(fullName) == employees.end())
    {
        throw std::invalid_argument("invalid argument detected in FindLongestContract() functon!");
    }
    else
    {
        int longest_contract_number = 0;
        std::time_t start_date;
        std::time_t end_date;
        std::time_t longest_holder = 0;
        for (const auto& contract : employees[fullName].contracts) 
        {
            parseDate(start_date, contract.startDate);
            parseDate(end_date, contract.endDate);
            if (end_date-start_date > longest_holder)
            {
                longest_holder = end_date-start_date;
                longest_contract_number = contract.number;
            }
        }

        std::cout <<"For " << fullName <<" contract #" << longest_contract_number << " is the longest contract." << std::endl<< std::endl;
    }
}

void Employee::FindMostExpensiveContract(const std::string& fullName, std::map<std::string, Employee>& employees) 
{
    if (fullName.empty() || employees.empty()|| employees.find(fullName) == employees.end())
    {
        throw std::invalid_argument("invalid argument detected in FindMostExpensiveContract() functon!");
    }
    else
    {
        int most_expensive_contract_number = 0;
        int most_expensive_contract_cost = 0;
        for (const auto& contract : employees[fullName].contracts) 
        {
            if (contract.cost > most_expensive_contract_cost)
            {
                most_expensive_contract_number = contract.number;
                most_expensive_contract_cost = contract.cost;
            }
        }

        std::cout <<"For " << fullName <<" contract #" << most_expensive_contract_number << " is the most expensive contract. Cost: " << most_expensive_contract_cost << std::endl << std::endl;
    }
}


void Employee::parseDate(std::time_t &date_to, const std::string& date_from)
{
    int date_day, date_month, date_year;
    char dot; 

    std::stringstream ss(date_from);
    ss >> date_day >> dot >> date_month>> dot >> date_year;

    std::tm timeinfo = {};
    timeinfo.tm_year = date_year - 1900; // год начинается с 1900
    timeinfo.tm_mon = date_month - 1; // месяц начинается с 0
    timeinfo.tm_mday = date_day;

    date_to = std::mktime(&timeinfo);
}

