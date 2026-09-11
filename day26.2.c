#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    int id;
    char name[50];
    int age;
    char course[50];
};

struct Student students[100];
int count = 0;

void addStudent() {
    struct Student s;
    printf("\nEnter Student ID: ");
    scanf("%d", &s.id);
    printf("Enter Name: ");
    scanf(" %[^\n]", s.name);
    printf("Enter Age: ");
    scanf("%d", &s.age);
    printf("Enter Course: ");
    scanf(" %[^\n]", s.course);

    students[count++] = s;
    printf("\n✅ Student added successfully!\n");
}

void displayStudents() {
    if (count == 0) {
        printf("\n⚠️ No records found!\n");
        return;
    }
    printf("\n--- Student Records ---\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d | Name: %s | Age: %d | Course: %s\n",
               students[i].id, students[i].name, students[i].age, students[i].course);
    }
}

void searchStudent() {
    int id;
    printf("\nEnter Student ID to search: ");
    scanf("%d", &id);
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            printf("\nFound: ID: %d | Name: %s | Age: %d | Course: %s\n",
                   students[i].id, students[i].name, students[i].age, students[i].course);
            return;
        }
    }
    printf("\n⚠️ Student not found!\n");
}

void deleteStudent() {
    int id;
    printf("\nEnter Student ID to delete: ");
    scanf("%d", &id);
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            for (int j = i; j < count - 1; j++) {
                students[j] = students[j + 1];
            }
            count--;
            printf("\n🗑️ Student deleted successfully!\n");
            return;
        }
    }
    printf("\n⚠️ Student not found!\n");
}

int main() {
    int choice;
    do {
        printf("\n===== Student Record Management System =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Delete Student\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: deleteStudent(); break;
            case 5: printf("\n👋 Exiting... Goodbye!\n"); break;
            default: printf("\n⚠️ Invalid choice! Try again.\n");
        }
    } while (choice != 5);

    return 0;
}
