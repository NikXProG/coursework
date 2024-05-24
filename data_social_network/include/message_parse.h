#include <iostream>
#include <fstream>
#include <chrono>
#include <set>
#include <string>
#include <sstream>
#include <memory>

static class MessageParse{

public:

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
        for (auto it = messages.begin(); it != messages.end(); ++it) {
            if ((*it)->username == message->username && 
                (*it)->time == message->time && 
                (*it)->info == message->info) {
                messages.erase(it);
                break; 
            }
        }
    }


};
