#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"

#define MAX_STUDENTS 100
#define MAX_NAME_LEN 100
#define MAX_PHONE_LEN 20
#define MAX_ID_LEN 20
#define MAX_DEPT_LEN 50
#define INPUT_BUFFER_SIZE 256

struct Student {
    char name[MAX_NAME_LEN];
    char phone[MAX_PHONE_LEN];
    char id[MAX_ID_LEN];
    char department[MAX_DEPT_LEN];
};

struct Student students[MAX_STUDENTS];
int studentCount = 0;

void pause();
void clearScreen();
void setUpTerminal();
void displayHeader();
void displayMenu();

void getStringInput(const char *prompt, char *buffer, int bufferSize);
int getIntInput(const char *prompt);
void tolower_string(char *str);

int isIdUnique(const char *id);
void addStudent();
void displayStudents();
void deleteStudent();
void searchStudentByName();
void modifyStudent();

int main() {
    setUpTerminal();
    int choice;
    while (1) {
        clearScreen();
        displayHeader();
        displayMenu();

        choice = getIntInput(CYAN "Enter your choice: " RESET);

        switch (choice) {
            case 1:
                addStudent();
                pause();
                break;
            case 2:
                displayStudents();
                pause();
                break;
            case 3:
                deleteStudent();
                pause();
                break;
            case 4:
                searchStudentByName();
                pause();
                break;
            case 5:
                modifyStudent();
                pause();
                break;
            case 6:
                printf(GREEN "\nExiting. Thank you for using our system!\n" RESET);
                return 0;
            default:
                printf(RED "\n[ERROR] Invalid choice. Please enter a number between 1 and 6.\n" RESET);
                pause();
                break;
        }
    }
    return 0;
}

void pause() {
    printf(YELLOW "\nPress Enter to continue..." RESET);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void setUpTerminal() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004;
    SetConsoleMode(hOut, dwMode);
#endif
}

void displayHeader() {
    printf(BLUE);
    printf("==========================================\n");
    printf("        DAFFODIL INTERNATIONAL UNIVERSITY\n");
    printf("           Student Entry System\n");
    printf("==========================================\n");
    printf("         Yunus Khan Scholar Garden-2\n");
    printf("==========================================\n\n" RESET);
}

void displayMenu() {
    printf(CYAN);
    printf("\n============ MAIN MENU ============\n");
    printf("1. Add Student\n");
    printf("2. View All Students\n");
    printf("3. Delete Student by ID\n");
    printf("4. Search Student by Name\n");
    printf("5. Modify Student by ID\n");
    printf("6. Exit\n");
    printf("===================================\n" RESET);
}

void getStringInput(const char *prompt, char *buffer, int bufferSize) {
    printf("%s", prompt);
    if (fgets(buffer, bufferSize, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
    } else {
        buffer[0] = '\0';
    }
}

int getIntInput(const char *prompt) {
    char inputBuffer[INPUT_BUFFER_SIZE];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(inputBuffer, sizeof(inputBuffer), stdin) != NULL) {
            if (sscanf(inputBuffer, "%d", &value) == 1) {
                return value;
            } else {
                printf(RED "[ERROR] Invalid input. Please enter a number.\n" RESET);
            }
        } else {
            printf(RED "Error reading input. Exiting.\n" RESET);
            exit(1);
        }
    }
}

void tolower_string(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int isIdUnique(const char *id) {
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(students[i].id, id) == 0) {
            return 0;
        }
    }
    return 1;
}

void addStudent() {
    if (studentCount >= MAX_STUDENTS) {
        printf(RED "\n[WARNING] Maximum student limit reached! Cannot add more students.\n" RESET);
        return;
    }

    struct Student newStudent;
    char tempBuffer[INPUT_BUFFER_SIZE];

    printf(CYAN "\n--- Add New Student ---\n" RESET);

    do {
        getStringInput(YELLOW "Enter student name: " RESET, newStudent.name, sizeof(newStudent.name));
        if (strlen(newStudent.name) == 0) {
            printf(RED "Name cannot be empty. Please enter a name.\n" RESET);
        }
    } while (strlen(newStudent.name) == 0);

    do {
        getStringInput(YELLOW "Enter phone number: " RESET, newStudent.phone, sizeof(newStudent.phone));
        if (strlen(newStudent.phone) == 0) {
            printf(RED "Phone number cannot be empty. Please enter a phone number.\n" RESET);
        }
    } while (strlen(newStudent.phone) == 0);

    do {
        getStringInput(YELLOW "Enter student ID: " RESET, newStudent.id, sizeof(newStudent.id));
        if (strlen(newStudent.id) == 0) {
            printf(RED "Student ID cannot be empty. Please enter an ID.\n" RESET);
        } else if (!isIdUnique(newStudent.id)) {
            printf(RED "[ERROR] Student ID '%s' already exists. Please enter a unique ID.\n" RESET, newStudent.id);
        }
    } while (strlen(newStudent.id) == 0 || !isIdUnique(newStudent.id));

    do {
        getStringInput(YELLOW "Enter department: " RESET, newStudent.department, sizeof(newStudent.department));
        if (strlen(newStudent.department) == 0) {
            printf(RED "Department cannot be empty. Please enter a department.\n" RESET);
        }
    } while (strlen(newStudent.department) == 0);

    students[studentCount] = newStudent;
    studentCount++;
    printf(GREEN "\n[OK] Student added successfully!\n" RESET);
}

void displayStudents() {
    if (studentCount == 0) {
        printf(RED "\n[WARNING] No student records found.\n" RESET);
        return;
    }

    printf(CYAN "\n--- Student List (%d records) ---\n" RESET, studentCount);
    for (int i = 0; i < studentCount; i++) {
        printf(MAGENTA "\nStudent %d:\n", i + 1);
        printf("  Name       : %s\n", students[i].name);
        printf("  Phone      : %s\n", students[i].phone);
        printf("  ID         : %s\n", students[i].id);
        printf("  Department : %s\n" RESET, students[i].department);
    }
}

void deleteStudent() {
    char delId[MAX_ID_LEN];
    char confirmation[5];
    printf(CYAN "\n--- Delete Student ---\n" RESET);
    getStringInput(YELLOW "Enter Student ID to delete: " RESET, delId, sizeof(delId));

    int foundIndex = -1;
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(students[i].id, delId) == 0) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex != -1) {
        printf(YELLOW "\nAre you sure you want to delete student with ID '%s' (Name: %s)? (yes/no): " RESET,
               students[foundIndex].id, students[foundIndex].name);
        getStringInput("", confirmation, sizeof(confirmation));
        tolower_string(confirmation);

        if (strcmp(confirmation, "yes") == 0) {
            for (int j = foundIndex; j < studentCount - 1; j++) {
                students[j] = students[j + 1];
            }
            studentCount--;
            printf(GREEN "\n[OK] Student with ID %s deleted successfully!\n" RESET, delId);
        } else {
            printf(BLUE "\nOperation cancelled. Student not deleted.\n" RESET);
        }
    } else {
        printf(RED "\n[ERROR] Student with ID '%s' not found.\n" RESET, delId);
    }
}

void searchStudentByName() {
    char searchName[MAX_NAME_LEN];
    char lowerSearchName[MAX_NAME_LEN];
    printf(CYAN "\n--- Search Student by Name ---\n" RESET);
    getStringInput(YELLOW "Enter name (or part of name) to search: " RESET, searchName, sizeof(searchName));

    strncpy(lowerSearchName, searchName, sizeof(lowerSearchName) - 1);
    lowerSearchName[sizeof(lowerSearchName) - 1] = '\0';
    tolower_string(lowerSearchName);

    int found = 0;
    printf(CYAN "\n--- Search Results ---\n" RESET);
    for (int i = 0; i < studentCount; i++) {
        char lowerStudentName[MAX_NAME_LEN];
        strncpy(lowerStudentName, students[i].name, sizeof(lowerStudentName) - 1);
        lowerStudentName[sizeof(lowerStudentName) - 1] = '\0';
        tolower_string(lowerStudentName);

        if (strstr(lowerStudentName, lowerSearchName)) {
            found = 1;
            printf(MAGENTA "\nStudent %d (ID: %s):\n", i + 1, students[i].id);
            printf("  Name       : %s\n", students[i].name);
            printf("  Phone      : %s\n", students[i].phone);
            printf("  ID         : %s\n", students[i].id);
            printf("  Department : %s\n" RESET, students[i].department);
        }
    }

    if (!found) {
        printf(RED "\n[INFO] No matching student found for '%s'.\n" RESET, searchName);
    }
}

void modifyStudent() {
    char modId[MAX_ID_LEN];
    char tempBuffer[INPUT_BUFFER_SIZE];
    printf(CYAN "\n--- Modify Student Details ---\n" RESET);
    getStringInput(YELLOW "Enter Student ID to modify: " RESET, modId, sizeof(modId));

    int foundIndex = -1;
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(students[i].id, modId) == 0) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex != -1) {
        printf(GREEN "\nStudent found (Name: %s). Enter new details (leave blank to keep current value):\n" RESET, students[foundIndex].name);

        getStringInput(YELLOW "Enter new name (current: %s): " RESET, tempBuffer, sizeof(tempBuffer));
        if (strlen(tempBuffer) > 0) {
            strncpy(students[foundIndex].name, tempBuffer, sizeof(students[foundIndex].name) - 1);
            students[foundIndex].name[sizeof(students[foundIndex].name) - 1] = '\0';
        }

        getStringInput(YELLOW "Enter new phone number (current: %s): " RESET, tempBuffer, sizeof(tempBuffer));
        if (strlen(tempBuffer) > 0) {
            strncpy(students[foundIndex].phone, tempBuffer, sizeof(students[foundIndex].phone) - 1);
            students[foundIndex].phone[sizeof(students[foundIndex].phone) - 1] = '\0';
        }

        getStringInput(YELLOW "Enter new department (current: %s): " RESET, tempBuffer, sizeof(tempBuffer));
        if (strlen(tempBuffer) > 0) {
            strncpy(students[foundIndex].department, tempBuffer, sizeof(students[foundIndex].department) - 1);
            students[foundIndex].department[sizeof(students[foundIndex].department) - 1] = '\0';
        }

        printf(GREEN "\n[OK] Student with ID %s updated successfully!\n" RESET, modId);
    } else {
        printf(RED "\n[ERROR] Student with ID '%s' not found.\n" RESET, modId);
    }
}
