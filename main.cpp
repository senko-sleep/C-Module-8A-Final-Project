#include "Library.h"
#include <iostream>

// starts the program
int main() {
    Library lib;
    
    try {
        lib.loadData();
        std::cout << "Data loaded successfully." << std::endl;
    } catch (const LibraryException& e) {
        std::cout << "Error loading files: " << e.what() << std::endl;
        std::cout << "Starting with empty library." << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }
    
    lib.runMenu();
    
    std::cout << "Program ended." << std::endl;
    return 0;
}