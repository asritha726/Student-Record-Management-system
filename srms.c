#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"
#define MAX_SUBJECTS 3

struct Student {
    int roll;
    char name[50];
    int age;
    char contact[15];

    // Course info
    char degree[30];
    char branch[30];
    char semester[20];

    // Academic info
    char subjects[MAX_SUBJECTS][50];
    float marks[MAX_SUBJECTS];
    char grades[MAX_SUBJECTS][5];
    float attendance;

    // Fee info
    float fees;
    char feeStatus[10];
};

char currentUser[20];
char currentRole[10];

int loginSystem();
void mainMenu();
void adminMenu();
void userMenu();
void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

int main() {
    if (loginSystem()) {
        mainMenu();
    } else {
        printf("\nLogin failed. Exiting...\n");
    }
    return 0;
}

// ================= Login System =================
int loginSystem() {
    char username[20], password[20];
    char fileUser[20], filePass[20], fileRole[10];

    printf("============ Login Screen ==============\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("Error: credentials.txt not found!\n");
        return 0;
    }

    while (fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) == 3) {
        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            strcpy(currentRole, fileRole);
            strcpy(currentUser, fileUser);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

// ================= Menu =================
void mainMenu() {
    if (strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else
        userMenu();
}

void adminMenu() {
    int choice;
    do {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: return;
            default: printf("Invalid Choice!\n");
        }
    } while (1);
}

void userMenu() {
    int choice;
    printf("\n===== USER MENU =====\n");
    printf("1. Display Students\n");
    printf("2. Search Student\n");
    printf("3. Logout\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: displayStudents(); break;
        case 2: searchStudent(); break;
        default: return;
    }
}

// ================= Add Student =================
void addStudent() {
    FILE *fp = fopen(STUDENT_FILE, "a");
    if (!fp) {
        printf("File error!\n");
        return;
    }

    struct Student st;

    printf("Enter Roll No: ");
    scanf("%d", &st.roll);
    printf("Enter Name: ");
    scanf("%s", st.name);
    printf("Enter Age: ");
    scanf("%d", &st.age);
    printf("Enter Contact: ");
    scanf("%s", st.contact);

    printf("Enter Degree: ");
    scanf("%s", st.degree);
    printf("Enter Branch: ");
    scanf("%s", st.branch);
    printf("Enter Semester/Year: ");
    scanf("%s", st.semester);

    for (int i = 0; i < MAX_SUBJECTS; i++) {
        printf("Enter Subject %d Name: ", i + 1);
        scanf("%s", st.subjects[i]);

        printf("Enter Marks for %s: ", st.subjects[i]);
        scanf("%f", &st.marks[i]);

        printf("Enter Grade for %s: ", st.subjects[i]);
        scanf("%s", st.grades[i]);
    }

    printf("Enter Attendance Percentage: ");
    scanf("%f", &st.attendance);

    printf("Enter Fees Amount: ");
    scanf("%f", &st.fees);

    printf("Enter Fee Status (Paid/Unpaid): ");
    scanf("%s", st.feeStatus);

    // Save to file
    fprintf(fp, "%d %s %d %s %s %s %s ",
            st.roll, st.name, st.age, st.contact,
            st.degree, st.branch, st.semester);

    for (int i = 0; i < MAX_SUBJECTS; i++) {
        fprintf(fp, "%s %.2f %s ",
                st.subjects[i], st.marks[i], st.grades[i]);
    }

    fprintf(fp, "%.2f %.2f %s\n",
            st.attendance, st.fees, st.feeStatus);

    fclose(fp);
    printf("Student Added Successfully!\n");
}

// ================= Display Students =================
void displayStudents() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("No records found!\n");
        return;
    }

    struct Student st;

    while (fscanf(fp, "%d %s %d %s %s %s %s",
                  &st.roll, st.name, &st.age, st.contact,
                  st.degree, st.branch, st.semester) == 7) {

        for (int i = 0; i < MAX_SUBJECTS; i++) {
            fscanf(fp, "%s %f %s",
                   st.subjects[i], &st.marks[i], st.grades[i]);
        }

        fscanf(fp, "%f %f %s",
               &st.attendance, &st.fees, st.feeStatus);

        printf("\n---------------------------------\n");
        printf("Roll: %d\nName: %s\nAge: %d\nContact: %s\n",
               st.roll, st.name, st.age, st.contact);
        printf("Degree: %s | Branch: %s | Semester: %s\n",
               st.degree, st.branch, st.semester);

        printf("Subject-wise Marks:\n");
        for (int i = 0; i < MAX_SUBJECTS; i++) {
            printf("%s - %.2f (%s)\n",
                   st.subjects[i], st.marks[i], st.grades[i]);
        }

        printf("Attendance: %.2f%%\n", st.attendance);
        printf("Fees: %.2f | Status: %s\n",
               st.fees, st.feeStatus);
    }
    fclose(fp);
}

// ================= Search Student =================
void searchStudent() {
    int r, found = 0;
    struct Student st;

    printf("Enter Roll No to search: ");
    scanf("%d", &r);

    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("File not found!\n");
        return;
    }

    while (fscanf(fp, "%d %s %d %s %s %s %s",
                  &st.roll, st.name, &st.age, st.contact,
                  st.degree, st.branch, st.semester) == 7) {

        for (int i = 0; i < MAX_SUBJECTS; i++) {
            fscanf(fp, "%s %f %s",
                   st.subjects[i], &st.marks[i], st.grades[i]);
        }

        fscanf(fp, "%f %f %s",
               &st.attendance, &st.fees, st.feeStatus);

        if (st.roll == r) {
            printf("\nRecord Found!\n");
            printf("Name: %s\nDegree: %s\nBranch: %s\nSemester: %s\n",
                   st.name, st.degree, st.branch, st.semester);

            printf("Marks:\n");
            for (int i = 0; i < MAX_SUBJECTS; i++) {
                printf("%s - %.2f (%s)\n",
                       st.subjects[i], st.marks[i], st.grades[i]);
            }

            printf("Attendance: %.2f%%\n", st.attendance);
            printf("Fees: %.2f | Status: %s\n",
                   st.fees, st.feeStatus);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Record Not Found!\n");

    fclose(fp);
}

// ================= Update Student =================
void updateStudent() {
    int r, found = 0;
    struct Student st;

    printf("Enter Roll No to update: ");
    scanf("%d", &r);

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    while (fscanf(fp, "%d %s %d %s %s %s %s",
                  &st.roll, st.name, &st.age, st.contact,
                  st.degree, st.branch, st.semester) == 7) {

        for (int i = 0; i < MAX_SUBJECTS; i++) {
            fscanf(fp, "%s %f %s",
                   st.subjects[i], &st.marks[i], st.grades[i]);
        }

        fscanf(fp, "%f %f %s",
               &st.attendance, &st.fees, st.feeStatus);

        if (st.roll == r) {
            found = 1;
            printf("Updating record...\n");

            printf("Enter New Name: ");
            scanf("%s", st.name);

            for (int i = 0; i < MAX_SUBJECTS; i++) {
                printf("Enter New Marks for %s: ", st.subjects[i]);
                scanf("%f", &st.marks[i]);

                printf("Enter New Grade: ");
                scanf("%s", st.grades[i]);
            }
        }

        fprintf(temp, "%d %s %d %s %s %s %s ",
                st.roll, st.name, st.age, st.contact,
                st.degree, st.branch, st.semester);

        for (int i = 0; i < MAX_SUBJECTS; i++) {
            fprintf(temp, "%s %.2f %s ",
                    st.subjects[i], st.marks[i], st.grades[i]);
        }

        fprintf(temp, "%.2f %.2f %s\n",
                st.attendance, st.fees, st.feeStatus);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Record Updated Successfully!\n");
    else
        printf("Record Not Found!\n");
}

// ================= Delete Student =================
void deleteStudent() {
    int r, found = 0;
    struct Student st;

    printf("Enter Roll No to delete: ");
    scanf("%d", &r);

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    while (fscanf(fp, "%d %s %d %s %s %s %s",
                  &st.roll, st.name, &st.age, st.contact,
                  st.degree, st.branch, st.semester) == 7) {

        for (int i = 0; i < MAX_SUBJECTS; i++) {
            fscanf(fp, "%s %f %s",
                   st.subjects[i], &st.marks[i], st.grades[i]);
        }

        fscanf(fp, "%f %f %s",
               &st.attendance, &st.fees, st.feeStatus);

        if (st.roll != r) {
            fprintf(temp, "%d %s %d %s %s %s %s ",
                    st.roll, st.name, st.age, st.contact,
                    st.degree, st.branch, st.semester);

            for (int i = 0; i < MAX_SUBJECTS; i++) {
                fprintf(temp, "%s %.2f %s ",
                        st.subjects[i], st.marks[i], st.grades[i]);
            }

            fprintf(temp, "%.2f %.2f %s\n",
                    st.attendance, st.fees, st.feeStatus);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);
    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Record Deleted Successfully!\n");
    else
        printf("Record Not Found!\n");
}
