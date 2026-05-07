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

void Library::saveData() {
    try {
        saveBooksToCSV();
        savePatronsToCSV();
    } catch (const std::exception& e) {
        throw LibraryException("Error saving data: " + std::string(e.what()));
    }
}

// template for saving items to csv
template<typename T, typename U>
void Library::saveToCSV(const std::string& filename, const T& items, U saver) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw LibraryException("Cannot open file for writing: " + filename);
    }
    for (const auto& item : items) {
        saver(file, item);
    }
    file.close();
}

// saves books as csv
void Library::saveBooksToCSV() {
    saveToCSV(booksFile, books, [](std::ofstream& file, Book* book) {
        std::string type = dynamic_cast<EBook*>(book) ? "EBook" : "PrintedBook";
        file << type << "," << book->getTitle() << "," << book->getAuthor() << ","
             << genreToString(book->getGenre()) << ",";
        
        if (auto ebook = dynamic_cast<EBook*>(book)) {
            file << ebook->getFileSize();
        } else if (auto pbook = dynamic_cast<PrintedBook*>(book)) {
            file << pbook->getPageCount();
        }
        
        file << "," << statusToString(book->getStatus()) << std::endl;
    });
}

// saves patrons as csv
void Library::savePatronsToCSV() {
    saveToCSV(patronsFile, patrons, [](std::ofstream& file, const std::pair<const int, Patron>& pair) {
        file << pair.first << "," << pair.second.getName() << std::endl;
    });
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
    patron->borrowBook(book);
    
    Transaction t(patronId, title, "checkout", Date::getCurrentDate());
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
    patron->returnBook(book);
    
    Transaction t(patronId, title, "return", Date::getCurrentDate());
    transactions.push_back(t);
    logTransaction(t);
}

void Library::searchByAuthor(const std::string& author) const {
    std::cout << "\n=== Books by Author: " << author << " ===" << std::endl;
    bool found = false;
    for (const auto& book : books) {
        if (book->getAuthor() == author) {
            std::cout << *book;
            found = true;
        }
    }
    if (!found) std::cout << "No books found by this author." << std::endl;
}

void Library::searchByGenre(Genre genre) const {
    std::cout << "\n=== Books by Genre: " << genreToString(genre) << " ===" << std::endl;
    bool found = false;
    for (const auto& book : books) {
        if (book->getGenre() == genre) {
            std::cout << *book;
            found = true;
        }
    }
    if (!found) std::cout << "No books found in this genre." << std::endl;
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
        std::cout << "7. Search Books" << std::endl;
        std::cout << "8. View Patron Details" << std::endl;
        std::cout << "9. Exit" << std::endl;
        std::cout << "Enter choice: ";
        
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }
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
                        while (!(std::cin >> fileSize)) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Invalid input. Please enter a numeric value for file size: ";
                        }
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        book = new EBook(title, author, genre, fileSize);
                    } else if (type == "PrintedBook") {
                        int pageCount;
                        std::cout << "Enter page count: ";
                        while (!(std::cin >> pageCount)) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Invalid input. Please enter a numeric value for page count: ";
                        }
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
                    while (!(std::cin >> id)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid ID. Please enter a numeric value for patron ID: ";
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    
                    Patron patron(name, id);
                    addPatron(patron);
                    std::cout << "Patron added successfully." << std::endl;
                    break;
                }
                case 5: {
                    int patronId;
                    std::string title;
                    std::cout << "Enter patron ID: ";
                    while (!(std::cin >> patronId)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid ID. Please enter a numeric value for patron ID: ";
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
                    while (!(std::cin >> patronId)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid ID. Please enter a numeric value for patron ID: ";
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter book title: ";
                    std::getline(std::cin, title);
                    
                    returnBook(patronId, title);
                    std::cout << "Book returned successfully." << std::endl;
                    break;
                }
                case 7: {
                    std::cout << "Search by: 1. Author, 2. Genre: ";
                    int searchChoice;
                    while (!(std::cin >> searchChoice)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input. Please enter 1 or 2: ";
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    if (searchChoice == 1) {
                        std::string author;
                        std::cout << "Enter author name: ";
                        std::getline(std::cin, author);
                        searchByAuthor(author);
                    } else if (searchChoice == 2) {
                        std::string genreStr;
                        std::cout << "Enter genre (FICTION, NON_FICTION, MYSTERY, ROMANCE, SCI_FI, BIOGRAPHY): ";
                        std::getline(std::cin, genreStr);
                        searchByGenre(stringToGenre(genreStr));
                    } else {
                        std::cout << "Invalid search option." << std::endl;
                    }
                    break;
                }
                case 8: {
                    int patronId;
                    std::cout << "Enter patron ID: ";
                    while (!(std::cin >> patronId)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid ID. Please enter a numeric value for patron ID: ";
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    Patron* p = findPatron(patronId);
                    if (p) p->displayPatron();
                    else std::cout << "Patron not found." << std::endl;
                    break;
                }
                case 9:
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
        file << t.getDate() << "," << t.getPatronId() << "," 
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