#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>

// records what happens with books
class Transaction {
private:
    int patronId;
    std::string bookTitle;
    std::string action; // "checkout" or "return"
    std::time_t timestamp;

public:
    Transaction(int pid, const std::string& bt, const std::string& act);
    
    int getPatronId() const { return patronId; }
    const std::string& getBookTitle() const { return bookTitle; }
    const std::string& getAction() const { return action; }
    std::time_t getTimestamp() const { return timestamp; }
};

#endif // TRANSACTION_H