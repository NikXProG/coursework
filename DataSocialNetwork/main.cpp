#include <iostream>
#include <fstream>
#include <chrono>
#include <set>
#include <string>
#include <sstream>
#include <memory>
#include <iomanip>

struct Message{

    std::string username;
    std::chrono::system_clock::time_point time;
    std::string info;
};

std::chrono::system_clock::time_point parseTime(std::string const& timeStr) {
    int hours, minutes, seconds, milliseconds;
    char colon;
    std::stringstream timeStream(timeStr);
    timeStream >> hours >> colon >> minutes >> colon >> seconds >> colon >> milliseconds;

    auto timeSinceEpoch = std::chrono::hours(hours) +
                          std::chrono::minutes(minutes) +
                          std::chrono::seconds(seconds) +
                          std::chrono::milliseconds(milliseconds);

    return std::chrono::system_clock::time_point(timeSinceEpoch);
}

void PrintUserMessage(const std::set<std::shared_ptr<Message>>& messages, const std::string& username) {
    for (const auto& msg : messages) {
        if (msg->username == username) {
            std::cout << msg->info << std::endl;
        }
    }
}

void PrintUserMessageInterval(const std::set<std::shared_ptr<Message>>& messages, const std::string& username,
                              const std::chrono::system_clock::time_point& start, const std::chrono::system_clock::time_point& end) {
    for (const auto& msg : messages) {
        if (msg->username == username && msg->time >= start && msg->time <= end) {
            std::cout << msg->info << std::endl;
        }
    }
}

void PrintMessageInterval(const std::set<std::shared_ptr<Message>>& messages, const std::chrono::system_clock::time_point& start,
                          const std::chrono::system_clock::time_point& end) {
    for (const auto& msg : messages) {
        if (msg->time >= start && msg->time <= end) {
            std::cout << msg->info << std::endl;
        }
    }
}

void RemoveUserMessage(std::set<std::shared_ptr<Message>>& messages, const std::string& username) {
    for (auto it = messages.begin(); it != messages.end();) {
        if ((*it)->username == username) {
            it = messages.erase(it);
        } else {
            ++it;
        }
    }
}

void RemoveMessage(std::set<std::shared_ptr<Message>>& messages, const std::shared_ptr<Message>& message) {
    auto msg = messages.find(message);
    if (msg != messages.end()) {
        messages.erase(msg);
    }
}

int main() {
    std::ifstream file("file.txt");

    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    std::set<std::shared_ptr<Message>> messages;
    std::string line;

    while (getline(file, line)) {
        size_t size = line.size();
        size_t first_space = line.find(' ');
        size_t last_colon = line.rfind(':');
        
        std::string userName = line.substr(0, first_space);
        std::string timeStr = line.substr(first_space + 1, last_colon - first_space - 1);
        std::string info = line.substr(last_colon + 1);

        auto time = parseTime(timeStr);
        auto msg = std::make_shared<Message>(Message{userName, time, info});
        messages.insert(msg);
    }

    std::string username = "Alice";
    auto start = parseTime("12:45:00:000");
    auto end = parseTime("13:48:00:000");

    PrintUserMessageInterval(messages, username, start, end);

    std::cout << std::endl;

    username = "Bob";

    PrintUserMessage(messages, username);

    std::cout << std::endl;

    username = "Alice";

    PrintMessageInterval(messages,  start, end);

    std::cout << std::endl;

    username = "Alice";

    RemoveUserMessage(messages,  username);
    
    for(const auto& msg: messages){
        std::cout << msg->username  << " " << msg->info << std::endl;
    }

    std::cout << std::endl;

    username = "Bob";
    auto time = parseTime("10:45:35:23"); 
    auto msg = std::make_shared<Message>(Message{username ,time , "Hi, Alice! How are you?"});
    RemoveMessage(messages, msg);
    
    for(const auto& msg: messages){
        std::cout << msg->username  << " " << msg->info << std::endl;
    }

    return 0;
}