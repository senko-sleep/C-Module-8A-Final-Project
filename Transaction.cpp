#include "Transaction.h"

// logs a transaction with current time
Transaction::Transaction(int pid, const std::string& bt, const std::string& act)
    : patronId(pid), bookTitle(bt), action(act), timestamp(std::time(nullptr)) {}