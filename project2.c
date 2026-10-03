/*
   Contact list management using structures in C
   Features: add, view, and search contacts
*/
#include <stdio.h>
#include <string.h>

#define MAX_CONTACTS 100

struct Contact {
    char name[100];
    char phone[20];
    char email[100];
};

void remove_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int main() {
    struct Contact contacts[MAX_CONTACTS];
    int count = 0;
    int choice;
    char searchName[100];

    do {
        printf("\n===== CONTACT LIST MENU =====\n");
        printf("1. Add Contact\n");
        printf("2. View Contacts\n");
        printf("3. Search Contact\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                if (count < MAX_CONTACTS) {
                    printf("Enter name: ");
                    fgets(contacts[count].name, sizeof(contacts[count].name), stdin);
                    remove_newline(contacts[count].name);

                    printf("Enter phone number: ");
                    fgets(contacts[count].phone, sizeof(contacts[count].phone), stdin);
                    remove_newline(contacts[count].phone);

                    printf("Enter email: ");
                    fgets(contacts[count].email, sizeof(contacts[count].email), stdin);
                    remove_newline(contacts[count].email);

                    count++;
                    printf("Contact added successfully!\n");
                } else {
                    printf("Contact list is full.\n");
                }
                break;

            case 2:
                if (count == 0) {
                    printf("No contacts available.\n");
                } else {
                    printf("\n--- Contact List ---\n");
                    for (int i = 0; i < count; i++) {
                        printf("Contact %d\n", i + 1);
                        printf("Name : %s\n", contacts[i].name);
                        printf("Phone: %s\n", contacts[i].phone);
                        printf("Email: %s\n\n", contacts[i].email);
                    }
                }
                break;

            case 3:
                if (count == 0) {
                    printf("No contacts available to search.\n");
                    break;
                }

                printf("Enter name to search: ");
                fgets(searchName, sizeof(searchName), stdin);
                remove_newline(searchName);

                int found = 0;
                for (int i = 0; i < count; i++) {
                    if (strcmp(contacts[i].name, searchName) == 0) {
                        printf("\nContact found!\n");
                        printf("Name : %s\n", contacts[i].name);
                        printf("Phone: %s\n", contacts[i].phone);
                        printf("Email: %s\n", contacts[i].email);
                        found = 1;
                        break;
                    }
                }

                if (!found) {
                    printf("No contact found with the name '%s'.\n", searchName);
                }
                break;

            case 4:
                printf("Exiting program. Goodbye!\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }

    } while (choice != 4);

    return 0;
}
