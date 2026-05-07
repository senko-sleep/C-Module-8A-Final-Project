#include "Book.h"

// sets up a book with basic info
Book::Book(const std::string& t, const std::string& a, Genre g)
    : title(t), author(a), genre(g), status(AVAILABLE) {}

EBook::EBook(const std::string& t, const std::string& a, Genre g, double fs)
    : Book(t, a, g), fileSize(fs) {}

PrintedBook::PrintedBook(const std::string& t, const std::string& a, Genre g, int pc)
    : Book(t, a, g), pageCount(pc) {}

// lets us print books easily
std::ostream& operator<<(std::ostream& os, const Book& book) {
    book.displayInfo(os);
    return os;
}

// shows ebook details
void EBook::displayInfo(std::ostream& os) const {
    os << "EBook - Title: " << title << ", Author: " << author 
       << ", Genre: " << genreToString(genre) 
       << ", File Size: " << fileSize << " MB, Status: " << statusToString(status) << std::endl;
}

// shows printed book details
void PrintedBook::displayInfo(std::ostream& os) const {
    os << "PrintedBook - Title: " << title << ", Author: " << author 
       << ", Genre: " << genreToString(genre) 
       << ", Pages: " << pageCount << ", Status: " << statusToString(status) << std::endl;
}

// turns genre enum into readable string
std::string genreToString(Genre g) {
    switch(g) {
        case FICTION: return "FICTION";
        case NON_FICTION: return "NON_FICTION";
        case MYSTERY: return "MYSTERY";
        case ROMANCE: return "ROMANCE";
        case SCI_FI: return "SCI_FI";
        case BIOGRAPHY: return "BIOGRAPHY";
        default: return "UNKNOWN";
    }
}

// turns status enum into readable string
std::string statusToString(BookStatus s) {
    switch(s) {
        case AVAILABLE: return "AVAILABLE";
        case CHECKED_OUT: return "CHECKED_OUT";
        case RESERVED: return "RESERVED";
        default: return "UNKNOWN";
    }
}