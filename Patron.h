#ifndef PATRON_H
#define PATRON_H

#include <string>
#include <iostream>

// represents a library user
class Patron {
private:
    std::string name;
    int id;

public:
    Patron(const std::string& n, int i);
    
    const std::string& getName() const { return name; }
    int getId() const { return id; }
    
    // checks if two patrons are the same by id
    bool operator==(const Patron& other) const {
        return id == other.id;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Patron& p);
};

#endif // PATRON_H