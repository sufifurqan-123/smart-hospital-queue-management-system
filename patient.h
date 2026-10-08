#ifndef PATIENT_H
#define PATIENT_H

#define ENCRYPTION_KEY 0x5A 

typedef struct {
    int id;
    char name[30];
    char specialization[30];
    int is_available;
} Doctor;

typedef struct Patient {
    int id;
    char name[50];
    int age;
    char gender[10];
    char disease[50];
    char phone[15];
    int is_emergency;
    char assigned_doctor[30];
    float bill_amount; 
    struct Patient* next; 
} Patient;

void addPatient();
void displayPatients();
void updatePatient();
void deletePatient();
void searchPatient();  
void settleBilling();   
void loadRecords();
void saveRecords();
void freeAllocatedMemory(); // Final Optimization Feature
void clearInputBuffer();
void initializeDoctors();
void displayDoctors();

#endif