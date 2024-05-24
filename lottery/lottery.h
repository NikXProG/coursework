#ifndef LOTTERY_H
#define LOTTERY_H

#include <iostream>
#include <vector>
#include <forward_list>
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

public:

    virtual void generateTickets() = 0;
    virtual void conductDraw() = 0;

public:

    const Ticket& getDrawnNumbers() const {
        return drawn_numbers;
    }

    const std::unordered_map<int, std::vector<Ticket>>& getWinners() const {
        return winners;
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

#endif // LOTTERY_H


/*template <typename Ticket>
class LotteryVector : public Lottery{

    LotteryVector : Lottery();

    void generateTickets()  override{
        for (int i = 0; i < total_tickets; ++i) {
            // заполнение билетов.
            Ticket ticket;
            int size_ticket = ticket.size();
            while (size_ticket < draw_numbers) {
                int number = std::rand() % total_numbers + 1;

                if (std::find(ticket.begin(), ticket.end(), number) == ticket.end()) {
                    ticket.push_back(number);
                }
            }

            tickets.push_back(ticket);
        }      
    }
    void conductDraw() override{

        Ticket drawn_numbers;

        int size = drawn_numbers.size();
        while (size < draw_numbers) {

            int number = std::rand() % total_numbers + 1;

            if (std::find(drawn_numbers.begin(), drawn_numbers.end(), number) == drawn_numbers.end()) {
                drawn_numbers.push_back(number);
            }
        
        }

        for (const auto& ticket : tickets) {
            int matched_numbers = 0;
            for (const auto& num : ticket) {
                if (std::find(drawn_numbers.begin(), drawn_numbers.end(), num) != drawn_numbers.end()) {
                    ++matched_numbers;
                }
            }
            if (matched_numbers > 0) {
                winners[matched_numbers].push_back(ticket);
            }
        }
        this->drawnNumbers = drawn_numbers;
    }
};*/