#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unistd.h> 

using namespace std;

void displayWelcomeMessage() {
    cout << "\nWelcome to the Library Management System!\n";
    cout << "\nGet ready to dive into the world of books and knowledge!\n";
}

struct Book {
    string title;
    string author;
    string ISBN;
    bool available;
};

bool isISBNExists(const vector<Book> &catalog, const string &ISBN) {
    for (const auto &book : catalog) {
        if (book.ISBN == ISBN) {
            return true;
        }
    }
    return false;
}

void addBook(vector<Book> &catalog) {
    Book newBook;
    cout << "Enter the title of the book: ";
    getline(cin, newBook.title);
    
    for (const auto &book : catalog) {
        if (book.title == newBook.title) {
            cout << "A book with the same title already exists in the catalog.\n";
            cout << "Please enter the ISBN of the book: ";
            getline(cin, newBook.ISBN);
            cout << "Enter the author of the book: ";
            getline(cin, newBook.author);
            newBook.available = true;
            catalog.push_back(newBook);
            cout << "\nBook successfully added to the library!\n";
            return;
        }
    }
    
    cout << "Enter the author of the book: ";
    getline(cin, newBook.author);
    
    string newISBN;
    do {
        cout << "Enter the ISBN of the book: ";
        getline(cin, newISBN);
        if (isISBNExists(catalog, newISBN)) {
            cout << "This ISBN is already assigned to another book. Please enter a different ISBN.\n";
        }
    } while (isISBNExists(catalog, newISBN));
    newBook.ISBN = newISBN;
    
    newBook.available = true;
    catalog.push_back(newBook);
    cout << "\nBook successfully added to the library!\n";
}

void removeBook(vector<Book> &catalog, const string &criteria) {
    auto it = catalog.begin();
    while (it != catalog.end()) {
        if (it->title == criteria || it->ISBN == criteria) {
            it = catalog.erase(it);
            cout << "\nThe book has been removed from the library.\n";
            return;
        } else {
            ++it;
        }
    }
    cout << "\nSorry, the book was not found in the library.\n";
}

void checkAvailability(const vector<Book> &catalog, const string &criteria) {
    bool found = false;
    for (const auto &book : catalog) {
        if (book.title == criteria || book.ISBN == criteria) {
            cout << "\nThe book \"" << book.title << "\" is " << (book.available ? "available for borrowing.\n" : "currently checked out.\n");
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "\nSorry, the book was not found in the library.\n";
    }
}

void displayCatalog(const vector<Book> &catalog) {
    if (catalog.empty()) {
        cout << "\nThe library catalog is currently empty.\n";
    } else {
        cout << "\n===== Library Catalog =====\n";
        int count = 1;
        for (const auto &book : catalog) {
            cout << count << (count == 1 ? "st" : count == 2 ? "nd" : count == 3 ? "rd" : "th") << " Book:\n";
            cout << "Title: " << book.title << "\n";
            cout << "Author: " << book.author << "\n";
            cout << "ISBN: " << book.ISBN << "\n";
            cout << "Availability: " << (book.available ? "Available" : "Checked Out") << "\n\n";
            count++;
        }
    }
}

void saveCatalogToFile(const vector<Book> &catalog, const string &filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto &book : catalog) {
            file << "Title: " << book.title << "\n";
            file << "Author: " << book.author << "\n";
            file << "ISBN: " << book.ISBN << "\n";
            file << "Availability: " << (book.available ? "Available" : "Checked Out") << "\n\n";
        }
        cout << "\nThe library catalog has been successfully saved to file.\n";
    } else {
        cout << "\nError: Unable to save library catalog to file. Please try again later.\n";
    }
}

void loadCatalogFromFile(vector<Book> &catalog, const string &filename) {
    ifstream file(filename);
    if (file.is_open()) {
        catalog.clear();
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string title, author, ISBN, available;
            getline(iss, title, ':');
            getline(iss, title);
            getline(file, author, ':');
            getline(file, author);
            getline(file, ISBN, ':');
            getline(file, ISBN);
            getline(file, available, ':');
            getline(file, available);
            catalog.push_back({title, author, ISBN, (available == "Available")});
        }
        cout << "\nThe library catalog has been successfully loaded from file.\n";
    } else {
        cout << "\nWarning: The library catalog file does not exist. Starting with an empty catalog.\n";
    }
}

int main() {
    displayWelcomeMessage();

    vector<Book> catalog;
    string filename = "library_catalog.txt";

    loadCatalogFromFile(catalog, filename);

    int choice;
    do {
        cout << "\n========== Main Menu ==========\n";
        cout << "1. Add Book\n";
        cout << "2. Remove Book\n";
        cout << "3. Check Book Availability\n";
        cout << "4. Display Catalog\n";
        cout << "5. Save Catalog to File\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                addBook(catalog);
                break;
            case 2: {
                string criteria;
                cout << "\nEnter the title or ISBN of the book to remove: ";
                getline(cin, criteria);
                removeBook(catalog, criteria);
                break;
            }
            case 3: {
                string criteria;
                cout << "\nEnter the title or ISBN of the book to check availability: ";
                getline(cin, criteria);
                checkAvailability(catalog, criteria);
                break;
            }
            case 4:
                displayCatalog(catalog);
                break;
            case 5:
                saveCatalogToFile(catalog, filename);
                break;
            case 6:
                cout << "\nThank you for using the Library Management System. Goodbye!\n";
                sleep(5); 
                break;
            default:
                cout << "\nInvalid choice. Please try again.\n";
        }
    } while (choice != 6);

    saveCatalogToFile(catalog, filename);

    return 0;
}
