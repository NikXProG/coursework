#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>
#include <ctime>
#include <algorithm>
#include <forward_list>
#include <chrono>
#include <thread>
using namespace std::chrono_literals;


template<typename Ticket>
class Lottery {
protected:
    int total_count_tickets;
    int total_available_numbers;
    int draw_numbers;
    Ticket drawn_numbers;

public:

    Lottery(int total_count_tickets, int total_available_numbers, int draw_numbers)
        : total_count_tickets(total_count_tickets), total_available_numbers(total_available_numbers), draw_numbers(draw_numbers) {
            std::srand(std::time(0));
    }

    virtual void generateTickets() = 0;
    virtual void conductDraw() = 0;


};

template <typename Ticket>
class LotteryVector : public Lottery<Ticket> {

    std::vector<Ticket> tickets;
    std::unordered_map<int, std::vector<Ticket>> winners;
    
public:
    LotteryVector(int total_count_tickets, int total_available_numbers, int draw_numbers)
        : Lottery<Ticket>(total_count_tickets, total_available_numbers, draw_numbers) {}
    

    std::vector<Ticket> searchByTicketNumber(int ticketNumber) const {
        std::vector<Ticket> matchingTickets;
        for (const auto& ticket : this->tickets) {
            if (std::find(ticket.begin(), ticket.end(), ticketNumber) != ticket.end()) {
                matchingTickets.push_back(ticket);
            }
        }
        return matchingTickets;
    }

    std::vector<Ticket> searchByWinningAmount(int winningAmount) const {
        std::vector<Ticket> matchingTickets;
        for (const auto& [matched, tickets] : winners) {
            if (matched == winningAmount) {
                matchingTickets.insert(matchingTickets.end(), tickets.begin(), tickets.end());
            }
        }
        return matchingTickets;
    }

    void generateTickets() override {
        // генерация всех билетов лотереи
        for (int i = 0; i < this->total_count_tickets; i++) {
            Ticket ticket;
            while ( ticket.size() < this->draw_numbers ) {
                int number = std::rand() % this->total_available_numbers + 1; // генерация случайных билетов в лотерее.
                if (std::find(ticket.begin(), ticket.end(), number) == ticket.end() ) {
                    ticket.push_back(number);
                }
            }
            this->tickets.push_back(ticket);
        }
    }
    void printResults() {
        std::cout << "Drawn Numbers: ";
        for (const auto& num : this->drawn_numbers) {
            std::cout << num << " ";
        }
        std::cout << std::endl;

        for (const auto& [matched, tickets] : winners) {
            std::cout << "Tickets with " << matched << " matched numbers: " << tickets.size() << std::endl;
        }
    }
    void conductDraw() override {

        this->drawn_numbers.clear();  // Очистка перед генерацией новых номеров

        // генерация счастливых чисел для билетов
        while (this->drawn_numbers.size() < this->draw_numbers) {
            int number = std::rand() % this->total_available_numbers + 1;
            if (std::find(this->drawn_numbers.begin(), this->drawn_numbers.end(), number) == this->drawn_numbers.end()) {
                this->drawn_numbers.push_back(number);
            }
        }

        // проверка соответствия чисел на билетах и если он победитель то заносим в контейнер
        for (const auto& ticket : this->tickets) {
            int matched_numbers = 0;
            for (const auto& num : ticket) {
                if (std::find(this->drawn_numbers.begin(), this->drawn_numbers.end(), num) != this->drawn_numbers.end()) {
                    ++matched_numbers;
                }
            }
            if (matched_numbers > 0) {
                this->winners[matched_numbers].push_back(ticket);
            }
        }
    }
};
template <typename Ticket>
class LotteryForwardList : public Lottery<Ticket> {
protected:

    std::forward_list<Ticket> tickets;
    std::unordered_map<int, std::forward_list<Ticket>> winners;

public:
    LotteryForwardList(int total_count_tickets, int total_available_numbers, int draw_numbers)
        : Lottery<Ticket>(total_count_tickets, total_available_numbers, draw_numbers) {}

    void generateTickets() override {
        // Generating all lottery tickets
        for (int i = 0; i < this->total_count_tickets; ++i) {
            Ticket ticket;
            int ticketSize = 0;
            while (ticketSize < this->draw_numbers) {
                int number = std::rand() % this->total_available_numbers + 1;
                if (std::find(ticket.begin(), ticket.end(), number) == ticket.end()) {
                    ticket.push_front(number);
                    ++ticketSize;
                }
            }
            this->tickets.push_front(ticket);
        }
    }
    void printResults() {
        std::cout << "Drawn Numbers: ";
        for (const auto& num : this->drawn_numbers) {
            std::cout << num << " ";
        }
        std::cout << std::endl;

        for (const auto& [matched, tickets] : this->winners) {
            int count = 0;
            for (const auto& ticket : tickets) {
                ++count;
            }
            std::cout << "Tickets with " << matched << " matched numbers: " << count << std::endl;
        }
    }
    void conductDraw() override {
        this->drawn_numbers.clear();  // Clear before generating new numbers

        // Generating lucky numbers for the tickets
        while (std::distance(this->drawn_numbers.begin(), this->drawn_numbers.end()) < this->draw_numbers) {
            int number = std::rand() % this->total_available_numbers + 1;
            if (std::find(this->drawn_numbers.begin(), this->drawn_numbers.end(), number) == this->drawn_numbers.end()) {
                this->drawn_numbers.push_front(number);
            }
        }

        // Checking ticket numbers and adding winners to the container
        for (const auto& ticket : this->tickets) {
            int matched_numbers = 0;
            for (const auto& num : ticket) {
                if (std::find(this->drawn_numbers.begin(), this->drawn_numbers.end(), num) != this->drawn_numbers.end()) {
                    ++matched_numbers;
                }
            }
            if (matched_numbers > 0) {
                this->winners[matched_numbers].push_front(ticket);
            }
        }
    }
};



template<typename T>
struct Node {
    T data;
    Node<T>* next;

    Node(const T& data) : data(data), next(nullptr) {}
};

// Реализация стека на базе односвязного списка
template<typename T>
class Stack {
private:
    Node<T>* topNode;

public:
    Stack() : topNode(nullptr) {}

    ~Stack() {
        while (topNode != nullptr) {
            Node<T>* temp = topNode;
            topNode = topNode->next;
            delete temp;
        }
    }

    void push(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        newNode->next = topNode;
        topNode = newNode;
    }

    void pop() {
        if (topNode != nullptr) {
            Node<T>* temp = topNode;
            topNode = topNode->next;
            delete temp;
        }
    }

    T top() const {
        if (topNode != nullptr) {
            return topNode->data;
        }
        throw std::out_of_range("Stack is empty");
    }

    bool isEmpty() const {
        return topNode == nullptr;
    }
};


template<typename T>
class Queue {
private:
    Node<T>* frontNode;
    Node<T>* rearNode;

public:
    Queue() : frontNode(nullptr), rearNode(nullptr) {}

    ~Queue() {
        while (frontNode != nullptr) {
            Node<T>* temp = frontNode;
            frontNode = frontNode->next;
            delete temp;
        }
    }

    void enqueue(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (isEmpty()) {
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
    }

    void dequeue() {
        if (!isEmpty()) {
            Node<T>* temp = frontNode;
            frontNode = frontNode->next;
            delete temp;
            if (frontNode == nullptr) {
                rearNode = nullptr;
            }
        }
    }

    T front() const {
        if (!isEmpty()) {
            return frontNode->data;
        }
        throw std::out_of_range("Queue is empty");
    }

    bool isEmpty() const {
        return frontNode == nullptr;
    }
};
template <typename Ticket, template<typename> class Container>
class LotteryStack : public Lottery<Ticket> {
private:
    Container<Ticket> tickets;
    std::unordered_map<int, std::vector<Ticket>> winners;
    std::vector<int> drawn_numbers;

public:
    LotteryStack(int total_count_tickets, int total_available_numbers, int draw_numbers)
        : Lottery<Ticket>(total_count_tickets, total_available_numbers, draw_numbers) {}

    void generateTickets() override {
        for (int i = 0; i < this->total_count_tickets; ++i) {
            Ticket ticket;
            while (ticket.size() < this->draw_numbers) {
                int number = std::rand() % this->total_available_numbers + 1;
                if (std::find(ticket.begin(), ticket.end(), number) == ticket.end()) {
                    ticket.push_back(number);
                }
            }
            tickets.push(ticket);
        }
    }

    void printResults() {
        std::cout << "Drawn Numbers: ";
        for (const auto& num : drawn_numbers) {
            std::cout << num << " ";
        }
        std::cout << std::endl;

        for (const auto& [matched, tickets] : winners) {
            int count = 0;
            for (const auto& ticket : tickets) {
                ++count;
            }
            std::cout << "Tickets with " << matched << " matched numbers: " << count << std::endl;
        }
    }

    void conductDraw() override {
        drawn_numbers.clear();  // Clear before generating new numbers

        while (drawn_numbers.size() < this->draw_numbers) {
            int number = std::rand() % this->total_available_numbers + 1;
            if (std::find(drawn_numbers.begin(), drawn_numbers.end(), number) == drawn_numbers.end()) {
                drawn_numbers.push_back(number);
            }
        }

        // Check tickets and determine winners
        while (!tickets.isEmpty()) {
            Ticket ticket = tickets.top();
            tickets.pop();

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
    }
};

template <typename Ticket, template<typename> class Container>
class LotteryQueue : public Lottery<Ticket> {
private:

    Container<Ticket> tickets;
    std::unordered_map<int, std::vector<Ticket>> winners;
    std::vector<int> drawn_numbers;

public:
    LotteryQueue(int total_count_tickets, int total_available_numbers, int draw_numbers)
        : Lottery<Ticket>(total_count_tickets, total_available_numbers, draw_numbers) {}

    void generateTickets() override {
        for (int i = 0; i < this->total_count_tickets; ++i) {
            Ticket ticket;
            while (ticket.size() < this->draw_numbers) {
                int number = std::rand() % this->total_available_numbers + 1;
                if (std::find(ticket.begin(), ticket.end(), number) == ticket.end()) {
                    ticket.push_back(number);
                }
            }
            tickets.enqueue(ticket);
        }
    }

    void printResults() {
        std::cout << "Drawn Numbers: ";
        for (const auto& num : drawn_numbers) {
            std::cout << num << " ";
        }
        std::cout << std::endl;

        for (const auto& [matched, tickets] : winners) {
            int count = 0;
            for (const auto& ticket : tickets) {
                ++count;
            }
            std::cout << "Tickets with " << matched << " matched numbers: " << count << std::endl;
        }
    }

    void conductDraw() override {
        drawn_numbers.clear();  // Clear before generating new numbers

        while (drawn_numbers.size() < this->draw_numbers) {
            int number = std::rand() % this->total_available_numbers + 1;
            if (std::find(drawn_numbers.begin(), drawn_numbers.end(), number) == drawn_numbers.end()) {
                drawn_numbers.push_back(number);
            }
        }

        // Check tickets and determine winners
        while (!tickets.isEmpty()) {
            Ticket ticket = tickets.front();
            tickets.dequeue();

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
    }
};

int main() {
    try{
        const int total_count_tickets = 100000;
        const int total_available_numbers = 36;
        const int draw_numbers = 5;

        LotteryForwardList<std::forward_list<int>> lottery(total_count_tickets, total_available_numbers, draw_numbers);

        lottery.generateTickets();
        lottery.conductDraw();
        lottery.printResults();



        const int total_available_numbers2 = 36;
        const int draw_numbers2 = 5;

        std::cout << std::endl;

        LotteryVector<std::vector<int>> lottery2(total_count_tickets, total_available_numbers2, draw_numbers2);

        lottery2.generateTickets();
        lottery2.conductDraw();
        lottery2.printResults();

        int targetTicketNumber = 222222;

        auto matchingTicketsByNumber = lottery2.searchByTicketNumber(targetTicketNumber);

        for (const auto& ticket : matchingTicketsByNumber) {
            std::cout << "Ticket: ";
            for (const auto& number : ticket) {
                std::cout << number << " ";
            }
            std::cout << std::endl;
        }


        auto matchingTicketsByAmount = lottery2.searchByWinningAmount(5);

        for (const auto& ticket : matchingTicketsByAmount) {
            std::cout << "Ticket: ";
            for (const auto& number : ticket) {
                std::cout << number << " ";
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;


        LotteryStack<std::vector<int>, Stack> lottery3(total_count_tickets, total_available_numbers2, draw_numbers2);

        lottery3.generateTickets();
        lottery3.conductDraw();
        lottery3.printResults();

        std::cout << std::endl;



        LotteryQueue<std::vector<int>, Queue> lottery4(total_count_tickets, total_available_numbers2, draw_numbers2);

        lottery4.generateTickets();
        lottery4.conductDraw();
        lottery4.printResults();

    }catch(const std::exception& ex){
        std::cout << ex.what() << std::endl;
        return -1;
    }

    return 0;
}