#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <iostream>

// groups related constants together
enum Genre { FICTION, NON_FICTION, MYSTERY, ROMANCE, SCI_FI, BIOGRAPHY };

// keeps track of book availability
enum BookStatus { AVAILABLE, CHECKED_OUT, RESERVED };

// base class for all books - handles common stuff
class Book {
protected:
    std::string title;
    std::string author;
    Genre genre;
    BookStatus status;

public:
    Book(const std::string& t, const std::string& a, Genre g);
    
    virtual ~Book() = default;
    
    // lets subclasses show their info differently
    virtual void displayInfo(std::ostream& os) const = 0;
    
    const std::string& getTitle() const { return title; }
    const std::string& getAuthor() const { return author; }
    Genre getGenre() const { return genre; }
    BookStatus getStatus() const { return status; }
    
    void setStatus(BookStatus s) { status = s; }
    
    // checks if two books are the same based on title and author
    bool operator==(const Book& other) const {
        return title == other.title && author == other.author;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Book& book);
};

// ebook version - adds file size
class EBook : public Book {
private:
    double fileSize;

public:
    EBook(const std::string& t, const std::string& a, Genre g, double fs);
    
    void displayInfo(std::ostream& os) const override;
    
    double getFileSize() const { return fileSize; }
};

// printed book version - adds page count
class PrintedBook : public Book {
private:
    int pageCount;

public:
    PrintedBook(const std::string& t, const std::string& a, Genre g, int pc);
    
    void displayInfo(std::ostream& os) const override;
    
    int getPageCount() const { return pageCount; }
};

// helper functions for converting enums to strings
std::string genreToString(Genre g);
std::string statusToString(BookStatus s);

#endif // BOOK_H