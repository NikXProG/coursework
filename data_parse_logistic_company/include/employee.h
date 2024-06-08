#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <ctime>

struct Contract 
{
public:
    int number;
    std::string startDate;
    std::string endDate;
    int cost;
};

class Employee 
{
public:

    std::string fullName;
    std::vector<Contract> contracts;

    void calculateTotalCost(const std::string& fullName, std::map<std::string, Employee>& employees);

    void listContracts(const std::string& fullName, std::map<std::string, Employee>& employees);

    void FindLongestContract(const std::string& fullName, std::map<std::string, Employee>& employees);

    void FindMostExpensiveContract(const std::string& fullName, std::map<std::string, Employee>& employees);

private:

    void parseDate(std::time_t &date_to, const std::string& date_from);
};
