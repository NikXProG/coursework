#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>
#include <ctime>
#include <algorithm>

template<typename Ticket>
class Lottery {

protected:

    int total_tickets;
    int total_numbers;
    int draw_numbers;
    Ticket drawn_numbers;
    std::vector<Ticket> tickets;
    std::unordered_map<int, std::vector<Ticket>> winners;

public:
    Lottery(int total_tickets, int total_numbers, int draw_numbers)
        : total_tickets(total_tickets), total_numbers(total_numbers), draw_numbers(draw_numbers) {
        std::srand(std::time(0));
    }

    virtual void generateTickets() = 0;
    virtual void conductDraw() = 0;

    void printResults() {
        std::cout << "Drawn Numbers: ";
        for (const auto& num : drawn_numbers) {
            std::cout << num << " ";
        }
        std::cout << std::endl;

        for (const auto& [matched, tickets] : winners) {
            std::cout << "Tickets with " << matched << " matched numbers: " << tickets.size() << std::endl;
        }
    }
    
};

template <typename Ticket>
class LotteryVector : public Lottery<Ticket> {

public:

    LotteryVector(int total_tickets, int total_numbers, int draw_numbers)
        : Lottery<Ticket>(total_tickets, total_numbers, draw_numbers) {}

public:

    void generateTickets() override {
        for (int i = 0; i < this->total_tickets; ++i) {
            Ticket ticket;
            while (ticket.size() < this->draw_numbers) {
                int number = std::rand() % this->total_numbers + 1; // генерация случайных билетов в лотерее.
                if (std::find(ticket.begin(), ticket.end(), number) == ticket.end()) {
                    ticket.push_back(number);
                }
            }
            this->tickets.push_back(ticket);
        }
    }

    void conductDraw() override {

        Ticket drawn_numbers;

        int size_drawn_numbers = drawn_numbers.size();

        // генерация случайных чисел
        while ( size_drawn_numbers < this->draw_numbers) {
            int number = std::rand() % this->total_numbers + 1;
            if (std::find(drawn_numbers.begin(), drawn_numbers.end(), number) == drawn_numbers.end()) {
                drawn_numbers.push_back(number);
            }
        }

        // проверка соответствия чисел на билетах и победители

        for (const auto& ticket : this->tickets) {
            int matched_numbers = 0;
            for (const auto& num : ticket) {
                if (std::find(drawn_numbers.begin(), drawn_numbers.end(), num) != drawn_numbers.end()) {
                    ++matched_numbers;
                }
            }
            if (matched_numbers > 0) {
                this->winners[matched_numbers].push_back(ticket);
            }
        }

        this->drawn_numbers = drawn_numbers;
    }
};

int main(){
    const int total_tickets = 10000;
    const int total_numbers = 49;
    const int draw_numbers = 6;

    LotteryVector<std::vector<int>> lottery(total_tickets, total_numbers, draw_numbers);

    lottery.generateTickets();
    lottery.conductDraw();


    return 0;
}