# Library Management System

A simple C++ program to manage a library. It handles books (ebooks and printed), patrons, checkouts, returns, and saves data to CSV files.

## How to run

Compile: `g++ -std=c++17 -o library_system main.cpp Book.cpp Patron.cpp Transaction.cpp Library.cpp`

Run: `./library_system`

It uses a menu for operations. Files: books.txt, patrons.txt, transactions.log.

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