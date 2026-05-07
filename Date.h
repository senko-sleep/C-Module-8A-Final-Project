#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <string>

class Date {
private:
    int day, month, year;

public:
    Date(int d = 1, int m = 1, int y = 2024) : day(d), month(m), year(y) {}
    
    std::string toString() const {
        return std::to_string(month) + "/" + std::to_string(day) + "/" + std::to_string(year);
    }
    
    static Date getCurrentDate() {
        // Simple mock for current date
        return Date(7, 5, 2026);
    }
    
    static Date getDueDate(int daysFromNow) {
        // Simple mock for due date calculation
        return Date(21, 5, 2026);
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Date& d) {
        os << d.toString();
        return os;
    }
};

#endif // DATE_H
