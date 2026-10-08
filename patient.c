#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "patient.h"

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define CYAN    "\033[1;36m"
#define BOLD    "\033[1m"

Patient* head = NULL; 
Doctor staff[3] = {0}; 

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void initializeDoctors() {
    staff[0] = (Doctor){101, "Dr. Alice Smith", "Cardiology", 1};
    staff[1] = (Doctor){102, "Dr. Bob Jones", "Orthopedics", 1};
    staff[2] = (Doctor){103, "Dr. Clara Oswald", "General", 1};
}

void displayDoctors() {
    printf("\n" CYAN "--- CLINIC SPECIALIST ROSTER ---" RESET "\n");
    for (int i = 0; i < 3; i++) {
        printf("ID: %d | %-18s | Specialization: %-12s | Status: %s\n",
               staff[i].id, staff[i].name, staff[i].specialization,
               staff[i].is_available ? GREEN "AVAILABLE" RESET : RED "BUSY" RESET);
    }
}

void encryptDecrypt(char* data, int size) {
    for (int i = 0; i < size; i++) {
        data[i] ^= ENCRYPTION_KEY;
    }
}

void saveRecords() {
    FILE* file = fopen("secure_patients.dat", "wb");
    if (file == NULL) {
        printf(RED "\n[Error] Disk sync failure!\n" RESET);
        return;
    }
    Patient* current = head;
    while (current != NULL) {
        Patient temp = *current;
        temp.next = NULL; 
        encryptDecrypt(temp.name, sizeof(temp.name));
        encryptDecrypt(temp.disease, sizeof(temp.disease));
        encryptDecrypt(temp.phone, sizeof(temp.phone));
        fwrite(&temp, sizeof(Patient), 1, file);
        current = current->next;
    }
    fclose(file);
}

void loadRecords() {
    FILE* file = fopen("secure_patients.dat", "rb");
    if (file == NULL) return; 

    Patient temp;
    while (fread(&temp, sizeof(Patient), 1, file) == 1) {
        encryptDecrypt(temp.name, sizeof(temp.name));
        encryptDecrypt(temp.disease, sizeof(temp.disease));
        encryptDecrypt(temp.phone, sizeof(temp.phone));

        Patient* newNode = (Patient*)malloc(sizeof(Patient));
        if (newNode == NULL) continue;
        *newNode = temp;
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
        } else {
            Patient* curr = head;
            while (curr->next != NULL) curr = curr->next;
            curr->next = newNode;
        }
    }
    fclose(file);
}

void displayDashboard() {
    int total = 0, emergencies = 0;
    Patient* curr = head;
    while (curr != NULL) {
        total++;
        if (curr->is_emergency) emergencies++;
        curr = curr->next;
    }

    printf(CYAN "\n+-------------------------------------------------------+\n");
    printf("  " BOLD "SECURE DASHBOARD" RESET CYAN " | Active Queue: " BOLD "%d" RESET CYAN " | Emergencies: " RED BOLD "%d" RESET CYAN "\n", total, emergencies);
    if (head != NULL) {
        printf("  Next in Line: " YELLOW BOLD "%s" RESET CYAN " -> Assigned to: %s\n", head->name, head->assigned_doctor);
    } else {
        printf("  Next in Line: " GREEN "None (Queue Empty)" RESET CYAN "\n");
    }
    printf("+-------------------------------------------------------+\n" RESET);
}

void addPatient() {
    Patient* newNode = (Patient*)malloc(sizeof(Patient));
    if (newNode == NULL) {
        printf(RED "\n[Error] RAM allocation failed!\n" RESET);
        return;
    }

    printf("\n" CYAN "--- Secure Patient Registration ---" RESET "\n");
    printf("Enter Patient ID (Integer): ");
    if (scanf("%d", &newNode->id) != 1) {
        printf(RED "[Error] Invalid ID input!\n" RESET);
        clearInputBuffer();
        free(newNode);
        return;
    }
    clearInputBuffer();

    Patient* curr = head;
    while (curr != NULL) {
        if (curr->id == newNode->id) {
            printf(RED "[Error] ID %d is actively registered.\n" RESET, newNode->id);
            free(newNode);
            return;
        }
        curr = curr->next;
    }

    printf("Enter Name: ");
    fgets(newNode->name, sizeof(newNode->name), stdin);
    newNode->name[strcspn(newNode->name, "\n")] = 0;

    printf("Enter Age: ");
    if (scanf("%d", &newNode->age) != 1) {
        printf(RED "[Error] Invalid Age!\n" RESET);
        clearInputBuffer();
        free(newNode);
        return;
    }
    clearInputBuffer();

    printf("Enter Gender: ");
    fgets(newNode->gender, sizeof(newNode->gender), stdin);
    newNode->gender[strcspn(newNode->gender, "\n")] = 0;

    printf("Enter Sickness (e.g., heart, bone, general): ");
    fgets(newNode->disease, sizeof(newNode->disease), stdin);
    newNode->disease[strcspn(newNode->disease, "\n")] = 0;

    printf("Enter Phone Number: ");
    fgets(newNode->phone, sizeof(newNode->phone), stdin);
    newNode->phone[strcspn(newNode->phone, "\n")] = 0;

    printf("Is this a Critical Emergency? (1 = Yes, 0 = No): ");
    if (scanf("%d", &newNode->is_emergency) != 1) {
        newNode->is_emergency = 0;
    }
    clearInputBuffer();
    newNode->next = NULL;

    if (newNode->is_emergency) {
        newNode->bill_amount = 500.00; 
    } else {
        newNode->bill_amount = 75.00;  
    }

    char lowerDisease[50];
    strcpy(lowerDisease, newNode->disease);
    for (int i = 0; lowerDisease[i]; i++) lowerDisease[i] = tolower(lowerDisease[i]);

    if (strstr(lowerDisease, "heart") || strstr(lowerDisease, "cardio")) {
        strcpy(newNode->assigned_doctor, staff[0].name);
        newNode->bill_amount += 250.00; 
    } else if (strstr(lowerDisease, "bone") || strstr(lowerDisease, "fracture") || strstr(lowerDisease, "ortho")) {
        strcpy(newNode->assigned_doctor, staff[1].name);
        newNode->bill_amount += 150.00; 
    } else {
        strcpy(newNode->assigned_doctor, staff[2].name);
    }

    if (head == NULL || (newNode->is_emergency && !head->is_emergency)) {
        newNode->next = head;
        head = newNode;
    } else {
        curr = head;
        while (curr->next != NULL && !(newNode->is_emergency && !curr->next->is_emergency)) {
            curr = curr->next;
        }
        newNode->next = curr->next;
        curr->next = newNode;
    }

    saveRecords();
    printf(GREEN "\n[Success] Patient dynamically queued and financial profile created!\n" RESET);
}

void displayPatients() {
    if (head == NULL) {
        printf(YELLOW "\nNo patient profiles in memory cache.\n" RESET);
        return;
    }

    printf("\n" BOLD "======================================================================================================================================\n");
    printf("%-5s | %-18s | %-4s | %-8s | %-12s | %-10s | %-18s | %-10s | %-10s\n", 
           "ID", "Name", "Age", "Gender", "Sickness", "Status", "Assigned Doctor", "Est. Wait", "Current Bill");
    printf("======================================================================================================================================\n" RESET);

    Patient* curr = head;
    int index = 0;
    while (curr != NULL) {
        int waitTime = index * 15;
        if (curr->is_emergency) waitTime = 0;

        if (curr->is_emergency) {
            printf(RED "%-5d | %-18s | %-4d | %-8s | %-12s | %-10s | %-18s | Immediate  | $%-9.2f\n" RESET,
                   curr->id, curr->name, curr->age, curr->gender, curr->disease, "CRITICAL", curr->assigned_doctor, curr->bill_amount);
        } else {
            printf("%-5d | %-18s | %-4d | %-8s | %-12s | %-10s | %-18s | %d mins     | $%-9.2f\n",
                   curr->id, curr->name, curr->age, curr->gender, curr->disease, "Routine", curr->assigned_doctor, waitTime, curr->bill_amount);
        }
        curr = curr->next;
        index++;
    }
    printf(BOLD "======================================================================================================================================\n" RESET);
}

void searchPatient() {
    if (head == NULL) {
        printf(YELLOW "\nQueue is completely empty. Nothing to search.\n" RESET);
        return;
    }

    int choice, searchId, found = 0;
    char searchStr[50];

    printf("\n" CYAN "--- Search Patient Records ---" RESET "\n");
    printf("1. Search by Patient ID\n");
    printf("2. Search by Illness/Disease\n");
    printf("Select choice: ");
    if (scanf("%d", &choice) != 1) {
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    Patient* curr = head;

    if (choice == 1) {
        printf("Enter ID to look up: ");
        if (scanf("%d", &searchId) != 1) {
            clearInputBuffer();
            return;
        }
        clearInputBuffer();

        while (curr != NULL) {
            if (curr->id == searchId) {
                found = 1;
                printf(GREEN "\nMatch Found!\n" RESET "ID: %d | Name: %s | Condition: %s | Doctor: %s | Bill Due: $%.2f\n", 
                       curr->id, curr->name, curr->disease, curr->assigned_doctor, curr->bill_amount);
                break;
            }
            curr = curr->next;
        }
    } else if (choice == 2) {
        printf("Enter Illness keyword: ");
        fgets(searchStr, sizeof(searchStr), stdin);
        searchStr[strcspn(searchStr, "\n")] = 0;

        for (int i = 0; searchStr[i]; i++) searchStr[i] = tolower(searchStr[i]);

        printf(GREEN "\nMatching Records:\n" RESET);
        while (curr != NULL) {
            char tempDisease[50];
            strcpy(tempDisease, curr->disease);
            for (int i = 0; tempDisease[i]; i++) tempDisease[i] = tolower(tempDisease[i]);

            if (strstr(tempDisease, searchStr) != NULL) {
                found = 1;
                printf("ID: %d | Name: %s | Sickness: %s | Doctor: %s\n", 
                       curr->id, curr->name, curr->disease, curr->assigned_doctor);
            }
            curr = curr->next;
        }
    }

    if (!found) printf(RED "\nNo matching records found in the database.\n" RESET);
}

void settleBilling() {
    if (head == NULL) {
        printf(YELLOW "\nNo patients are currently checked in.\n" RESET);
        return;
    }

    int id, found = 0;
    printf("\nEnter Patient ID for Invoice Settlement: ");
    if (scanf("%d", &id) != 1) {
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    Patient* curr = head;
    while (curr != NULL) {
        if (curr->id == id) {
            found = 1;
            printf("\n" GREEN "=========================================" RESET);
            printf("\n" BOLD "       OFFICIAL MEDICAL INVOICE" RESET);
            printf("\n" GREEN "=========================================" RESET);
            printf("\n Patient Account ID : %d", curr->id);
            printf("\n Full Patient Name  : %s", curr->name);
            printf("\n Primary Diagnosis  : %s", curr->disease);
            printf("\n Attending Provider : %s", curr->assigned_doctor);
            printf("\n" CYAN "-----------------------------------------" RESET);
            printf("\n Total Balance Due  : " RED BOLD "$%.2f" RESET, curr->bill_amount);
            printf("\n" GREEN "=========================================" RESET);
            
            char payChoice;
            printf("\nProcess payment collection now? (y/n): ");
            scanf(" %c", &payChoice);
            
            if (payChoice == 'y' || payChoice == 'Y') {
                printf(GREEN "\n[Paid] Balance cleared out. Processing full discharge...\n" RESET);
                clearInputBuffer();
                
                Patient* innerCurr = head;
                Patient* prev = NULL;
                while (innerCurr != NULL) {
                    if (innerCurr->id == id) {
                        if (prev == NULL) head = innerCurr->next;
                        else prev->next = innerCurr->next;
                        free(innerCurr);
                        saveRecords();
                        break;
                    }
                    prev = innerCurr;
                    innerCurr = innerCurr->next;
                }
                printf(GREEN "[Success] Patient discharged cleanly from queue.\n" RESET);
            } else {
                printf(YELLOW "\n[Pending] Payment deferred. Profile remains active.\n" RESET);
                clearInputBuffer();
            }
            break;
        }
        curr = curr->next;
    }
    if (!found) printf(RED "\n[Error] Patient statement could not be located.\n" RESET);
}

void updatePatient() {
    int id, found = 0;
    printf("\nEnter Patient ID to update: ");
    if (scanf("%d", &id) != 1) {
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    Patient* curr = head;
    while (curr != NULL) {
        if (curr->id == id) {
            found = 1;
            printf(YELLOW "\nUpdating Patient Profile ID: %d\n" RESET, id);
            printf("Enter New Name: ");
            fgets(curr->name, sizeof(curr->name), stdin);
            curr->name[strcspn(curr->name, "\n")] = 0;

            printf("Enter New Age: ");
            if (scanf("%d", &curr->age) == 1) {
                clearInputBuffer();
            }
            saveRecords();
            printf(GREEN "\n[Success] Record updated and re-encrypted on disk.\n" RESET);
            break;
        }
        curr = curr->next;
    }
    if (!found) printf(RED "\n[Error] Patient profile lookup failed.\n" RESET);
}

void deletePatient() {
    int id, found = 0;
    printf("\nEnter Patient ID to discharge directly: ");
    if (scanf("%d", &id) != 1) {
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    Patient* curr = head;
    Patient* prev = NULL;

    while (curr != NULL) {
        if (curr->id == id) {
            found = 1;
            if (prev == NULL) head = curr->next;
            else prev->next = curr->next;
            printf(GREEN "\n[Success] Safely discharged: %s\n" RESET, curr->name);
            free(curr); 
            saveRecords();
            break;
        }
        prev = curr;
        curr = curr->next;
    }
    if (!found) printf(RED "\n[Error] Patient profile lookup failed.\n" RESET);
}

// Memory Destructor Engine to completely wipe lingering allocation leak footprints on stop
void freeAllocatedMemory() {
    Patient* curr = head;
    while (curr != NULL) {
        Patient* nextNode = curr->next;
        free(curr);
        curr = nextNode;
    }
    head = NULL;
}