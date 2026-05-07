#include "Transaction.h"
#include <iostream>

Transaction::Transaction(int pid, const std::string& bt, const std::string& act, const Date& d)
    : patronId(pid), bookTitle(bt), action(act), date(d) {}

void Transaction::displayTransaction() const {
    std::cout << "Date: " << date << " | Patron ID: " << patronId 
              << " | Book: " << bookTitle << " | Action: " << action << std::endl;
}