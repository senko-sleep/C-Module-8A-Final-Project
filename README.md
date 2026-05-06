# C++ Library Management System

## Overview

This is a comprehensive C++ Library Management System that demonstrates advanced Object-Oriented Programming (OOP) principles, including inheritance, polymorphism, encapsulation, and abstraction. The system manages books (both electronic and printed), patrons, and transactions with persistent storage and a menu-driven interface.

## Features

- **Book Management**: Support for both EBooks and PrintedBooks with different attributes
- **Patron Management**: Register and manage library patrons
- **Transaction Logging**: Automatic logging of all checkout and return operations
- **File I/O**: CSV-based persistence for books, patrons, and transaction logs
- **Exception Handling**: Robust error handling for invalid operations and file errors
- **Operator Overloading**: Custom equality and output operators for Book and Patron classes
- **Polymorphism**: Virtual methods for displaying book information
- **STL Usage**: Utilizes std::vector, std::map, and other standard library containers

## Architecture

### Class Hierarchy

```
Book (Abstract Base Class)
├── EBook
└── PrintedBook

Patron
Transaction
Library (Main Controller)
LibraryException (Custom Exception)
```

### Key OOP Principles Demonstrated

1. **Inheritance**: EBook and PrintedBook inherit from Book base class
2. **Polymorphism**: Virtual `displayInfo()` method overridden in derived classes
3. **Encapsulation**: Private member variables with public getter/setter methods
4. **Abstraction**: Book class provides abstract interface for different book types
5. **Composition**: Library class contains collections of Books and Patrons
6. **Exception Handling**: Custom LibraryException for domain-specific errors

## Building and Running

### Prerequisites

- C++17 compatible compiler (g++, clang++, etc.)
- Standard C++ libraries

### Compilation

```bash
g++ -std=c++17 -o library_system main.cpp Book.cpp Patron.cpp Transaction.cpp Library.cpp
```

### Execution

```bash
./library_system
```

## File Formats

### books.txt (CSV Format)

```
EBook,Title,Author,Genre,FileSize,Status
PrintedBook,Title,Author,Genre,PageCount,Status
```

Example:
```
EBook,The Great Gatsby,F. Scott Fitzgerald,FICTION,2.5,AVAILABLE
PrintedBook,1984,George Orwell,SCI_FI,328,CHECKED_OUT
```

### patrons.txt (CSV Format)

```
ID,Name
```

Example:
```
101,John Doe
102,Jane Smith
```

### transactions.log (CSV Format)

```
Timestamp,PatronID,BookTitle,Action
```

Example:
```
Wed May  6 10:30:45 2026,101,The Great Gatsby,checkout
```

## Usage

The system provides a menu-driven interface with the following options:

1. **View Books**: Display all books in the library with their details
2. **View Patrons**: Display all registered patrons
3. **Add Book**: Add a new book (EBook or PrintedBook) to the library
4. **Add Patron**: Register a new patron
5. **Checkout Book**: Checkout a book for a patron
6. **Return Book**: Return a checked-out book
7. **Exit**: Save data and exit the program

## Design Patterns Used

- **Template Method**: File I/O operations use template functions for generic CSV loading
- **Factory Pattern**: Book creation based on type string
- **Observer Pattern**: Transaction logging as a side effect of operations
- **Strategy Pattern**: Different display strategies for different book types

## Error Handling

The system includes comprehensive exception handling for:

- File I/O errors (missing files, permission issues)
- Invalid patron IDs
- Non-existent books
- Unavailable books for checkout
- Invalid input data during CSV parsing

## Memory Management

- Proper use of dynamic memory allocation for Book objects
- RAII (Resource Acquisition Is Initialization) in Library destructor
- Smart pointer usage avoided for simplicity but proper cleanup implemented

## Testing

See the test cases section below for comprehensive test scenarios to validate system functionality.

## Future Enhancements

- Database integration for better persistence
- GUI interface using Qt or similar framework
- Advanced search and filtering capabilities
- Reservation system for books
- Fine calculation for overdue books
- Multi-threading for concurrent operations

## Program Flowchart (Pseudocode)

```
BEGIN PROGRAM

CREATE Library object

TRY
    CALL loadData()
CATCH error
    DISPLAY "Error loading files"
    END PROGRAM
END TRY

WHILE TRUE

    DISPLAY "===== Library Menu ====="
    DISPLAY "1. View Books"
    DISPLAY "2. View Patrons"
    DISPLAY "3. Add Book"
    DISPLAY "4. Add Patron"
    DISPLAY "5. Checkout Book"
    DISPLAY "6. Return Book"
    DISPLAY "7. Exit"

    INPUT choice

    IF choice == 1 THEN
        CALL displayBooks()

    ELSE IF choice == 2 THEN
        CALL displayPatrons()

    ELSE IF choice == 3 THEN
        INPUT title, author, genre
        INPUT type (EBook or PrintedBook)

        IF type == "EBook" THEN
            INPUT fileSize
            CREATE new EBook
        ELSE
            INPUT pageCount
            CREATE new PrintedBook
        END IF

        ADD book to library

    ELSE IF choice == 4 THEN
        INPUT name, id
        CREATE new Patron
        ADD patron to library

    ELSE IF choice == 5 THEN
        INPUT patronId, bookTitle

        TRY
            CALL checkoutBook(patronId, bookTitle)
            DISPLAY "Book checked out successfully"
        CATCH error
            DISPLAY "Checkout failed: ", error
        END TRY

    ELSE IF choice == 6 THEN
        INPUT patronId, bookTitle

        TRY
            CALL returnBook(patronId, bookTitle)
            DISPLAY "Book returned successfully"
        CATCH error
            DISPLAY "Return failed: ", error
        END TRY

    ELSE IF choice == 7 THEN
        TRY
            CALL saveData()
            DISPLAY "Data saved successfully"
        CATCH error
            DISPLAY "Error saving data"
        END TRY

        BREAK LOOP

    ELSE
        DISPLAY "Invalid option"
    END IF

END WHILE

DISPLAY "Program Ended"

END PROGRAM
```

## Test Cases

### Test Case 1: Add and View Books
1. Start the program
2. Select option 3 (Add Book)
3. Enter: Title="The Great Gatsby", Author="F. Scott Fitzgerald", Genre="FICTION", Type="PrintedBook", PageCount=180
4. Select option 3 again
5. Enter: Title="Digital Fortress", Author="Dan Brown", Genre="MYSTERY", Type="EBook", FileSize=1.2
6. Select option 1 (View Books)
7. Verify both books are displayed with correct information

### Test Case 2: Add and View Patrons
1. Select option 4 (Add Patron)
2. Enter: Name="Alice Johnson", ID=101
3. Select option 4 again
4. Enter: Name="Bob Smith", ID=102
5. Select option 2 (View Patrons)
6. Verify both patrons are displayed

### Test Case 3: Successful Book Checkout
1. Select option 5 (Checkout Book)
2. Enter: PatronID=101, BookTitle="The Great Gatsby"
3. Verify "Book checked out successfully" message
4. Select option 1 (View Books)
5. Verify "The Great Gatsby" shows status CHECKED_OUT

### Test Case 4: Book Return
1. Select option 6 (Return Book)
2. Enter: PatronID=101, BookTitle="The Great Gatsby"
3. Verify "Book returned successfully" message
4. Select option 1 (View Books)
5. Verify "The Great Gatsby" shows status AVAILABLE

### Test Case 5: Checkout Unavailable Book
1. Select option 5 (Checkout Book)
2. Enter: PatronID=101, BookTitle="The Great Gatsby"
3. Select option 5 again
4. Enter: PatronID=102, BookTitle="The Great Gatsby"
5. Verify error message "Book 'The Great Gatsby' is not available"

### Test Case 6: Invalid Patron ID
1. Select option 5 (Checkout Book)
2. Enter: PatronID=999, BookTitle="Digital Fortress"
3. Verify error message "Patron with ID 999 not found"

### Test Case 7: Non-existent Book
1. Select option 5 (Checkout Book)
2. Enter: PatronID=101, BookTitle="Non-existent Book"
3. Verify error message "Book 'Non-existent Book' not found"

### Test Case 8: Data Persistence
1. Perform Test Cases 1-4
2. Select option 7 (Exit)
3. Restart the program
4. Select option 1 (View Books) and option 2 (View Patrons)
5. Verify all data was saved and loaded correctly

### Test Case 9: Transaction Logging
1. Perform checkout and return operations
2. Check transactions.log file
3. Verify all transactions are logged with correct timestamp, patron ID, book title, and action

### Test Case 10: Invalid Menu Option
1. Enter choice 99
2. Verify "Invalid option" message is displayed
3. Program continues running