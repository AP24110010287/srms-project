#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student {
    int roll;
    char name[50];
    float marks;
};

char currentRole[50];
char currentUser[50];

void mainMenu();
void adminMenu();
void staffMenu();
void guestMenu();
int loginSystem();

void createStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

int getValidInt();
float getValidFloat();
int checkRollExists(int roll);
void getString(char *buffer, int size);
void toLowerCase(char *str);

int main() {
    if (loginSystem()) {
        mainMenu();
    } else {
        printf("\nLogin failed. Exiting...\n");
    }
    return 0;
}

void getString(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int getValidInt() {
    char buffer[100];
    int value;
    char *endptr;
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            value = (int)strtol(buffer, &endptr, 10);
            if (endptr != buffer && (*endptr == '\n' || *endptr == '\0')) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number: ");
    }
}

float getValidFloat() {
    char buffer[100];
    float value;
    char *endptr;
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            value = strtof(buffer, &endptr);
            if (endptr != buffer && (*endptr == '\n' || *endptr == '\0')) {
                return value;
            }
        }
        printf("Invalid input. Please enter a numeric value: ");
    }
}

int checkRollExists(int roll) {
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) return 0;

    struct Student s;
    while (fscanf(fp, "%d|%49[^|]|%f\n", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == roll) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int loginSystem() {
    char username[50], password[50];
    char fileUser[50], filePass[50], fileRole[20];
    FILE *fp;
    int attempts = 0;

    while (attempts < 3) {
        printf("======= LOGIN ===========\n");
        printf("Username: ");
        getString(username, sizeof(username));

        printf("Password: ");
        getString(password, sizeof(password));

        fp = fopen(CREDENTIAL_FILE, "r");
        if (!fp) {
            printf("Error: %s not found.\n", CREDENTIAL_FILE);
            return 0;
        }

        int loginSuccess = 0;
        while (fscanf(fp, "%49s %49s %19s", fileUser, filePass, fileRole) == 3) {
            if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
                strcpy(currentRole, fileRole);
                strcpy(currentUser, fileUser);
                loginSuccess = 1;
                break;
            }
        }
        fclose(fp);

        if (loginSuccess) {
            printf("\nLogin Successful! Role: %s\n", currentRole);
            return 1;
        } else {
            printf("Invalid Credentials. Try again.\n");
            attempts++;
        }
    }
    return 0;
}

void mainMenu() {
    if (strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else if (strcmp(currentRole, "STAFF") == 0)
        staffMenu();
    else
        guestMenu();
}

void adminMenu() {
    int choice;
    do {
        printf("\n==== ADMIN MENU ====\n");
        printf("1. Create Student Record\n");
        printf("2. Display All Records\n");
        printf("3. Search Record\n");
        printf("4. Update Record\n");
        printf("5. Delete Record\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        
        choice = getValidInt();

        switch (choice) {
            case 1: createStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Logging out...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 6);
}

void staffMenu() {
    int choice;
    do {
        printf("\n==== STAFF MENU ====\n");
        printf("1. Display All Records\n");
        printf("2. Search Record\n");
        printf("3. Update Record\n");
        printf("4. Logout\n");
        printf("Enter choice: ");
        
        choice = getValidInt();

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: updateStudent(); break;
            case 4: printf("Logging out...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 4);
}

void guestMenu() {
    int choice;
    do {
        printf("\n==== GUEST MENU ====\n");
        printf("1. Display All Records\n");
        printf("2. Search Record\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        
        choice = getValidInt();

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: printf("Logging out...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 3);
}

void createStudent() {
    struct Student s;
    
    printf("Enter Roll Number: ");
    s.roll = getValidInt();

    if (checkRollExists(s.roll)) {
        printf("Error: Roll Number %d already exists!\n", s.roll);
        return;
    }

    FILE *fp = fopen(STUDENT_FILE, "a");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter Name: ");
    getString(s.name, sizeof(s.name));

    printf("Enter Marks: ");
    s.marks = getValidFloat();

    fprintf(fp, "%d|%s|%.2f\n", s.roll, s.name, s.marks);
    fclose(fp);
    printf("Student Created Successfully.\n");
}

void displayStudents() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    printf("\n%-10s %-30s %-10s\n", "ROLL", "NAME", "MARKS");
    printf("-----------------------------------------------------------\n");

    int count = 0;
    while (fscanf(fp, "%d|%49[^|]|%f\n", &s.roll, s.name, &s.marks) == 3) {
        printf("%-10d %-30s %-10.2f\n", s.roll, s.name, s.marks);
        count++;
    }
    fclose(fp);
    if (count == 0) printf("No records found in file.\n");
}

void searchStudent() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    int choice;
    printf("\nSearch By:\n");
    printf("1. Name\n");
    printf("2. Roll Number\n");
    printf("Enter choice: ");
    choice = getValidInt();

    int found = 0;

    if (choice == 1) {
        char searchName[50];
        char tempSearch[50];
        char tempName[50];

        printf("Enter Name to search: ");
        getString(searchName, sizeof(searchName));

        strcpy(tempSearch, searchName);
        toLowerCase(tempSearch);

        while (fscanf(fp, "%d|%49[^|]|%f\n", &s.roll, s.name, &s.marks) == 3) {
            strcpy(tempName, s.name);
            toLowerCase(tempName);

            if (strcmp(tempName, tempSearch) == 0) {
                if (!found) printf("\n--- Matching Records ---\n");
                printf("Roll: %d\nName: %s\nMarks: %.2f\n\n", s.roll, s.name, s.marks);
                found = 1;
            }
        }
        if (!found) printf("No student found with that name.\n");

    } else if (choice == 2) {
        int roll;
        printf("Enter Roll Number to search: ");
        roll = getValidInt();

        while (fscanf(fp, "%d|%49[^|]|%f\n", &s.roll, s.name, &s.marks) == 3) {
            if (s.roll == roll) {
                printf("\n--- Record Found ---\n");
                printf("Roll: %d\nName: %s\nMarks: %.2f\n", s.roll, s.name, s.marks);
                found = 1;
                break;
            }
        }
        if (!found) printf("Student not found.\n");
    } else {
        printf("Invalid choice.\n");
    }

    fclose(fp);
}

void updateStudent() {
    struct Student s;
    int roll, found = 0;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("Error opening files.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    printf("Enter Roll Number to update: ");
    roll = getValidInt();

    while (fscanf(fp, "%d|%49[^|]|%f\n", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == roll) {
            found = 1;
            printf("Current: %s, %.2f\n", s.name, s.marks);

            printf("Enter New Name: ");
            getString(s.name, sizeof(s.name));

            printf("Enter New Marks: ");
            s.marks = getValidFloat();

            fprintf(temp, "%d|%s|%.2f\n", s.roll, s.name, s.marks);
        } else {
            fprintf(temp, "%d|%s|%.2f\n", s.roll, s.name, s.marks);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(STUDENT_FILE);
        rename("temp.txt", STUDENT_FILE);
        printf("Record Updated Successfully.\n");
    } else {
        remove("temp.txt");
        printf("Record not found.\n");
    }
}

void deleteStudent() {
    struct Student s;
    int roll, found = 0;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("Error opening files.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    printf("Enter Roll Number to delete: ");
    roll = getValidInt();

    while (fscanf(fp, "%d|%49[^|]|%f\n", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == roll) {
            found = 1;
        } else {
            fprintf(temp, "%d|%s|%.2f\n", s.roll, s.name, s.marks);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(STUDENT_FILE);
        rename("temp.txt", STUDENT_FILE);
        printf("Record Deleted Successfully.\n");
    } else {
        remove("temp.txt");
        printf("Record not found.\n");
    }
}