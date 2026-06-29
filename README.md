# Smart Hospital Management & Triage Ecosystem

A production-ready, console-based hospital management application built completely from scratch in C. This system transitions away from static data limits by leveraging a dynamic singly linked list queue, automated specialist matchmaking, a point-of-sale billing matrix, and a cryptographic data-persistence layer.

---

## 📌 Features & Architecture

* **Dynamic RAM Allocation:** Uses a singly linked list (`malloc`/`free`) to handle infinite queue scaling with an automated destructor engine to eliminate memory leaks on system exit.
* **Smart Triage Priority Queue:** Critical emergency cases automatically bypass the line, restructuring the linked list pointers in real-time to bubble emergencies to the top.
* **Automated Doctor Matchmaking:** Parses string inputs of symptoms to auto-assign localized specialist providers (e.g., Cardiology, Orthopedics) based on condition matching.
* **HIPAA-Inspired Cryptography:** Implements a custom low-overhead XOR byte cipher that encrypts patient records before writing to disk, ensuring data persistence is fully human-unreadable from outside the software framework.
* **Integrated POS Billing Ledger:** Auto-calculates facility and diagnostic tier expenses based on patient triage level and specialization requirements.

---

## 🛠️ Tech Stack & Concepts Applied

* **Language:** C (C11 Standard)
* **Data Structures:** Dynamic Linked Lists, Priority Queues, Custom Type Structs
* **Security:** Symmetric Cryptographic Obfuscation (XOR Bitwise Logic)
* **Memory Management:** Dynamic Pointer Manipulation, Heap Allocation (`malloc`), Manual Garbage Cleanup (`free`)
* **File Handling:** Binary File I/O (`fopen`, `fwrite`, `fread`)

---

## 📁 File Structure

* `patient.h` — Blueprints, structure definitions, constants, and cryptographic key masks.
* `patient.c` — The core processing engine handling queue logic, cryptography, allocation, and sorting matrices.
* `main.c` — The driver program managing the console dashboard loop and UI state switches.

---

## 🚀 Getting Started

### Prerequisites
You need a C compiler installed on your local system (such as `gcc` or `clang`).

### Compilation & Execution
1. Clone this repository or download the source files into a single directory.
2. Open your terminal or command prompt inside the project directory.
3. Compile the system components using the following command:
   ```bash
   gcc main.c patient.c -o hospital_system
