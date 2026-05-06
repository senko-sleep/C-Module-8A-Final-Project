#include "Library.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>

// cleans up book pointers
Library::~Library() {
    for (auto book : books) {
        delete book;
    }
    books.clear();
}

// loads data from files
void Library::loadData() {
    try {
        loadFromCSV(booksFile, [this](std::istringstream& iss) {
            std::string type, title, author, genreStr, extra, statusStr;
            std::getline(iss, type, ',');
            std::getline(iss, title, ',');
            std::getline(iss, author, ',');
            std::getline(iss, genreStr, ',');
            std::getline(iss, extra, ',');
            std::getline(iss, statusStr);
            
            Genre genre = stringToGenre(genreStr);
            BookStatus status = stringToStatus(statusStr);
            
            Book* book = nullptr;
            if (type == "EBook") {
                double fileSize = std::stod(extra);
                book = new EBook(title, author, genre, fileSize);
            } else if (type == "PrintedBook") {
                int pageCount = std::stoi(extra);
                book = new PrintedBook(title, author, genre, pageCount);
            }
            
            if (book) {
                book->setStatus(status);
                books.push_back(book);
            }
        });
        
        loadFromCSV(patronsFile, [this](std::istringstream& iss) {
            std::string idStr, name;
            std::getline(iss, idStr, ',');
            std::getline(iss, name);
            
            int id = std::stoi(idStr);
            patrons[id] = Patron(name, id);
        });
        
    } catch (const std::exception& e) {
        throw LibraryException("Error loading data: " + std::string(e.what()));
    }
}

// template for loading csv files
template<typename T>
void Library::loadFromCSV(const std::string& filename, T loader) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw LibraryException("Cannot open file: " + filename);
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        loader(iss);
    }
    file.close();
}

// saves data to files
void Library::saveData() {
    try {
        saveBooksToCSV();
        savePatronsToCSV();
    } catch (const std::exception& e) {
        throw LibraryException("Error saving data: " + std::string(e.what()));
    }
}

// saves books as csv
void Library::saveBooksToCSV() {
    std::ofstream file(booksFile);
    if (!file.is_open()) {
        throw LibraryException("Cannot open file: " + booksFile);
    }
    
    for (const auto book : books) {
        std::string type = dynamic_cast<EBook*>(book) ? "EBook" : "PrintedBook";
        file << type << "," << book->getTitle() << "," << book->getAuthor() << ","
             << genreToString(book->getGenre()) << ",";
        
        if (auto ebook = dynamic_cast<EBook*>(book)) {
            file << ebook->getFileSize();
        } else if (auto pbook = dynamic_cast<PrintedBook*>(book)) {
            file << pbook->getPageCount();
        }
        
        file << "," << statusToString(book->getStatus()) << std::endl;
    }
    file.close();
}

// saves patrons as csv
void Library::savePatronsToCSV() {
    std::ofstream file(patronsFile);
    if (!file.is_open()) {
        throw LibraryException("Cannot open file: " + patronsFile);
    }
    
    for (const auto& pair : patrons) {
        file << pair.first << "," << pair.second.getName() << std::endl;
    }
    file.close();
}

// shows all books
void Library::displayBooks() const {
    if (books.empty()) {
        std::cout << "No books in the library." << std::endl;
        return;
    }
    
    std::cout << "\n=== Library Books ===" << std::endl;
    for (const auto book : books) {
        std::cout << *book;
    }
}

// shows all patrons
void Library::displayPatrons() const {
    if (patrons.empty()) {
        std::cout << "No patrons in the library." << std::endl;
        return;
    }
    
    std::cout << "\n=== Library Patrons ===" << std::endl;
    for (const auto& pair : patrons) {
        std::cout << pair.second << std::endl;
    }
}

// adds a book
void Library::addBook(Book* book) {
    if (!book) return;
    books.push_back(book);
}

// adds a patron
void Library::addPatron(const Patron& patron) {
    patrons[patron.getId()] = patron;
}

// checks out a book
void Library::checkoutBook(int patronId, const std::string& title) {
    Patron* patron = findPatron(patronId);
    if (!patron) {
        throw LibraryException("Patron with ID " + std::to_string(patronId) + " not found");
    }
    
    Book* book = findBook(title);
    if (!book) {
        throw LibraryException("Book '" + title + "' not found");
    }
    
    if (book->getStatus() != AVAILABLE) {
        throw LibraryException("Book '" + title + "' is not available");
    }
    
    book->setStatus(CHECKED_OUT);
    Transaction t(patronId, title, "checkout");
    transactions.push_back(t);
    logTransaction(t);
}

// returns a book
void Library::returnBook(int patronId, const std::string& title) {
    Patron* patron = findPatron(patronId);
    if (!patron) {
        throw LibraryException("Patron with ID " + std::to_string(patronId) + " not found");
    }
    
    Book* book = findBook(title);
    if (!book) {
        throw LibraryException("Book '" + title + "' not found");
    }
    
    if (book->getStatus() != CHECKED_OUT) {
        throw LibraryException("Book '" + title + "' is not checked out");
    }
    
    book->setStatus(AVAILABLE);
    Transaction t(patronId, title, "return");
    transactions.push_back(t);
    logTransaction(t);
}

// main menu loop
void Library::runMenu() {
    while (true) {
        std::cout << "\n===== Library Menu =====" << std::endl;
        std::cout << "1. View Books" << std::endl;
        std::cout << "2. View Patrons" << std::endl;
        std::cout << "3. Add Book" << std::endl;
        std::cout << "4. Add Patron" << std::endl;
        std::cout << "5. Checkout Book" << std::endl;
        std::cout << "6. Return Book" << std::endl;
        std::cout << "7. Exit" << std::endl;
        std::cout << "Enter choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        try {
            switch (choice) {
                case 1:
                    displayBooks();
                    break;
                case 2:
                    displayPatrons();
                    break;
                case 3: {
                    std::string title, author, genreStr, type;
                    std::cout << "Enter title: ";
                    std::getline(std::cin, title);
                    std::cout << "Enter author: ";
                    std::getline(std::cin, author);
                    std::cout << "Enter genre (FICTION, NON_FICTION, MYSTERY, ROMANCE, SCI_FI, BIOGRAPHY): ";
                    std::getline(std::cin, genreStr);
                    std::cout << "Enter type (EBook or PrintedBook): ";
                    std::getline(std::cin, type);
                    
                    Genre genre = stringToGenre(genreStr);
                    Book* book = nullptr;
                    
                    if (type == "EBook") {
                        double fileSize;
                        std::cout << "Enter file size (MB): ";
                        std::cin >> fileSize;
                        std::cin.ignore();
                        book = new EBook(title, author, genre, fileSize);
                    } else if (type == "PrintedBook") {
                        int pageCount;
                        std::cout << "Enter page count: ";
                        std::cin >> pageCount;
                        std::cin.ignore();
                        book = new PrintedBook(title, author, genre, pageCount);
                    } else {
                        std::cout << "Invalid book type." << std::endl;
                        break;
                    }
                    
                    addBook(book);
                    std::cout << "Book added successfully." << std::endl;
                    break;
                }
                case 4: {
                    std::string name;
                    int id;
                    std::cout << "Enter patron name: ";
                    std::getline(std::cin, name);
                    std::cout << "Enter patron ID: ";
                    std::cin >> id;
                    std::cin.ignore();
                    
                    Patron patron(name, id);
                    addPatron(patron);
                    std::cout << "Patron added successfully." << std::endl;
                    break;
                }
                case 5: {
                    int patronId;
                    std::string title;
                    std::cout << "Enter patron ID: ";
                    std::cin >> patronId;
                    std::cin.ignore();
                    std::cout << "Enter book title: ";
                    std::getline(std::cin, title);
                    
                    checkoutBook(patronId, title);
                    std::cout << "Book checked out successfully." << std::endl;
                    break;
                }
                case 6: {
                    int patronId;
                    std::string title;
                    std::cout << "Enter patron ID: ";
                    std::cin >> patronId;
                    std::cin.ignore();
                    std::cout << "Enter book title: ";
                    std::getline(std::cin, title);
                    
                    returnBook(patronId, title);
                    std::cout << "Book returned successfully." << std::endl;
                    break;
                }
                case 7:
                    saveData();
                    std::cout << "Data saved successfully. Exiting..." << std::endl;
                    return;
                default:
                    std::cout << "Invalid option. Please try again." << std::endl;
            }
        } catch (const LibraryException& e) {
            std::cout << "Error: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Unexpected error: " << e.what() << std::endl;
        }
    }
}

// finds a book by title
Book* Library::findBook(const std::string& title) {
    auto it = std::find_if(books.begin(), books.end(), 
        [&title](const Book* book) { return book->getTitle() == title; });
    return it != books.end() ? *it : nullptr;
}

// finds a patron by id
Patron* Library::findPatron(int id) {
    auto it = patrons.find(id);
    return it != patrons.end() ? &it->second : nullptr;
}

// logs a transaction to file
void Library::logTransaction(const Transaction& t) {
    std::ofstream file(logFile, std::ios::app);
    if (file.is_open()) {
        char timeStr[26];
        ctime_s(timeStr, sizeof(timeStr), &t.getTimestamp());
        timeStr[24] = '\0';
        
        file << timeStr << "," << t.getPatronId() << "," 
             << t.getBookTitle() << "," << t.getAction() << std::endl;
        file.close();
    }
}

// converts string to genre
Genre Library::stringToGenre(const std::string& str) {
    if (str == "FICTION") return FICTION;
    if (str == "NON_FICTION") return NON_FICTION;
    if (str == "MYSTERY") return MYSTERY;
    if (str == "ROMANCE") return ROMANCE;
    if (str == "SCI_FI") return SCI_FI;
    if (str == "BIOGRAPHY") return BIOGRAPHY;
    return FICTION;
}

// converts string to status
BookStatus Library::stringToStatus(const std::string& str) {
    if (str == "AVAILABLE") return AVAILABLE;
    if (str == "CHECKED_OUT") return CHECKED_OUT;
    if (str == "RESERVED") return RESERVED;
    return AVAILABLE;
}