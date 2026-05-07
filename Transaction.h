#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>
#include "Date.h"

// records what happens with books
class Transaction {
private:
    int patronId;
    std::string bookTitle;
    std::string action; // "checkout" or "return"
    Date date;

public:
    Transaction(int pid, const std::string& bt, const std::string& act, const Date& d);
    
    void displayTransaction() const;
    
    int getPatronId() const { return patronId; }
    const std::string& getBookTitle() const { return bookTitle; }
    const std::string& getAction() const { return action; }
    Date getDate() const { return date; }
};

#endif // TRANSACTION_H