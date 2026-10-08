#include <stdio.h>
#include <stdlib.h>
#include "patient.h"

void displayDashboard();
int main() {
    int choice;
    initializeDoctors();
    loadRecords();
    do {
        displayDashboard();
        printf("\n===================================\n");
        printf("    NEXT-GEN HOSPITAL ECOSYSTEM    \n");
        printf("===================================\n");
        printf("  1. Secure Register Patient\n");
        printf("  2. View Real-Time Crypt-Queue\n");
        printf("  3. Check Available Specialists\n");
        printf("  4. Search Patient Database\n");
        printf("  5. Settle Billing & Checkout\n");
        printf("  6. Modify Patient Core Record\n");
        printf("  7. Secure Log Out System\n");
        printf("===================================\n");
        printf("Select Action (1-7): ");
        
        if (scanf("%d", &choice) != 1) {
            printf("\n[Error] Select numeric items only.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: addPatient(); break;
            case 2: displayPatients(); break;
            case 3: displayDoctors(); break;
            case 4: searchPatient(); break;
            case 5: settleBilling(); break;
            case 6: updatePatient(); break;
            case 7: 
                printf("\nDestructing operational runtime cache...");
                freeAllocatedMemory(); // Deallocates all pointers safely right before shut down
                printf("\nData synced securely to disk. Goodbye!\n"); 
                break;
            default: printf("\n[Warning] Choice must range between 1 and 7.\n");
        }
    } while (choice != 7);

    return 0;
}