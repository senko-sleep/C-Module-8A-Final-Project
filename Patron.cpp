#include "Patron.h"

// creates a patron
Patron::Patron(const std::string& n, int i) : name(n), id(i) {}

// prints patron info nicely
std::ostream& operator<<(std::ostream& os, const Patron& p) {
    os << "ID: " << p.id << ", Name: " << p.name;
    return os;
}