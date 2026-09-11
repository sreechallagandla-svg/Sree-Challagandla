#include <stdio.h>
#include <string.h>

#define MAX_BOOKS 100

typedef struct {
    int id;
    char title[50];
    char author[50];
    int isIssued; // 0 = Available, 1 = Issued
} Book;

Book library[MAX_BOOKS];
int bookCount = 0;

void clearInput(void) {
    int character;
    while ((character = getchar()) != '\n' && character != EOF) {
    }
}

int readInteger(const char *prompt) {
    int value;

    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            clearInput();
            return value;
        }
        printf("Invalid input. Please enter a number.\n");
        clearInput();
    }
}

void readText(const char *prompt, char *text, size_t size) {
    printf("%s", prompt);
    if (fgets(text, (int)size, stdin) == NULL) {
        text[0] = '\0';
        return;
    }

    if (strchr(text, '\n') == NULL) {
        clearInput();
    }
    text[strcspn(text, "\n")] = '\0';
}

int findBook(int id) {
    for (int i = 0; i < bookCount; i++) {
        if (library[i].id == id) {
            return i;
        }
    }
    return -1;
}

void addBook() {
    int id;

    if (bookCount >= MAX_BOOKS) {
        printf("\nLibrary full!\n");
        return;
    }

    id = readInteger("\nEnter Book ID: ");
    if (findBook(id) != -1) {
        printf("A book with that ID already exists.\n");
        return;
    }

    library[bookCount].id = id;
    readText("Enter Title: ", library[bookCount].title, sizeof(library[bookCount].title));
    readText("Enter Author: ", library[bookCount].author, sizeof(library[bookCount].author));
    library[bookCount].isIssued = 0;
    bookCount++;
    printf("Book added successfully!\n");
}

void displayBooks() {
    if (bookCount == 0) {
        printf("\nNo books in library.\n");
        return;
    }
    printf("\n===================================================================\n");
    printf("%-8s | %-25s | %-20s | %-10s\n", "ID", "Title", "Author", "Status");
    printf("===================================================================\n");
    for (int i = 0; i < bookCount; i++) {
        printf("%-8d | %-25s | %-20s | %-10s\n", 
               library[i].id, library[i].title, library[i].author, 
               library[i].isIssued ? "Borrowed" : "Available");
    }
    printf("===================================================================\n");
}

void borrowBook() {
    int id = readInteger("\nEnter Book ID to borrow: ");
    int index = findBook(id);

    if (index == -1) {
        printf("Book not found.\n");
    } else if (library[index].isIssued) {
        printf("Book is already borrowed.\n");
    } else {
        library[index].isIssued = 1;
        printf("You borrowed '%s' successfully!\n", library[index].title);
    }
}

void returnBook() {
    int id = readInteger("\nEnter Book ID to return: ");
    int index = findBook(id);

    if (index == -1) {
        printf("Book not found.\n");
    } else if (!library[index].isIssued) {
        printf("Book was not borrowed.\n");
    } else {
        library[index].isIssued = 0;
        printf("You returned '%s' successfully!\n", library[index].title);
    }
}

int main() {
    int choice;
    do {
        printf("\n=== LIBRARY MANAGEMENT SYSTEM ===\n");
        printf("1. Add Book\n2. View Books (Tabular)\n3. Borrow Book\n4. Return Book\n5. Exit\n");
        choice = readInteger("Enter choice: ");
        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: borrowBook(); break;
            case 4: returnBook(); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 5);
    return 0;
}