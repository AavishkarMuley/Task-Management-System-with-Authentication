# Secure CLI Authentication and Task Management System

A high-performance, security-focused Command-Line Interface (CLI) application built from scratch in C++. This project pairs a **cryptographically-salted DJB2 authentication engine** with an **in-memory, user-isolated Task Management System** powered by a Doubly Linked List and Hash Map.

---

## 📌 Project Overview

Most beginner projects store passwords in plain text or keep all user tasks inside a single shared file. This project addresses both security and performance bottlenecks by implementing:
1. **Cryptographic Protection:** Prevents credential leaks and Rainbow Table attacks using salted DJB2 hash digests.
2. **Memory Security (Lazy Loading):** User records are authenticated on-demand from disk, ensuring inactive user data is never exposed in volatile RAM.
3. **High-Throughput Task Engine:** Employs a custom Doubly Linked List paired with a Hash Map (`std::unordered_map`) to guarantee constant-time $O(1)$ task operations.
4. **Data Isolation:** Automatically routes authenticated users to their own dedicated workspace storage files (`<username>.txt`), completely eliminating cross-user data interference.

---

## 🏗️ System Architecture & Workflow

```
               [ User Input: Username & Password ]
                                │
                                ▼
                     ┌──────────────────────┐
                     │  Read login.txt File │
                     └──────────┬───────────┘
                                │
             ┌──────────────────┴──────────────────┐
             ▼                                     ▼
      [ User Exists ]                      [ New User Detected ]
             │                                     │
     Fetch User's Salt                     Generate Random Salt
             │                                     │
   Compute DJB2 Hash                     Compute DJB2 Hash
  Hash(Password + Salt)                 Hash(Password + Salt)
             │                                     │
             ▼                                     ▼
   Compare Stored Hash                    Append New Record to
   with Computed Hash                         login.txt
             │                                     │
    ┌────────┴────────┐                            │
    ▼                 ▼                            │
[ Mismatch ]       [ Match ]                       │
    │                 │                            │
Access Denied!        └──────────────┬─────────────┘
                                     │
                                     ▼
                         [ Authentication Passed ]
                                     │
                                     ▼
                  Route to Dedicated User Workspace
                  (Loads Tasks into Doubly Linked List)
                                     │
            ┌────────────────────────┼────────────────────────┐
            ▼                        ▼                        ▼
       [Add Task]              [Delete Task]             [Save & Exit]
       O(1) Insert              O(1) Removal           File Persistence
```

---

## 🔑 Key Features

### 1. Authentication & Cryptographic Security
* **Salted DJB2 Hashing:** Uses Daniel J. Bernstein’s DJB2 algorithm ($hash \times 33 + c$) combined with a dynamically generated 5-character cryptographic salt per account.
* **Rainbow Table Resistance:** Identical passwords across different user accounts produce entirely distinct hash signatures.
* **Interactive Terminal Masking:** Intercepts real-time keystrokes using Windows `_getch()`, replacing visible characters with asterisks (`*`) while cleanly handling `Backspace` (`\b \b`) and `Enter` keys.
* **Case-Insensitive Identifier Handling:** Sanitizes usernames to lower-case, preventing username impersonation attacks and avoiding Windows OS file-naming collision bugs.

### 2. Task Management & Data Structures
* **$O(1)$ Insertion:** Adds tasks directly to the tail of the Doubly Linked List in constant time without traversing existing entries.
* **$O(1)$ Deletion by Name:** Maps task names directly to node pointers in an `std::unordered_map`, bypassing $O(N)$ linear scans when removing scheduled items.
* **File Serialization:** Tasks are persisted on disk using structured delimiter formatting (`TaskName|Time|Date|`), allowing seamless reconstruction during subsequent sessions.

---

## 📊 Data Structures & Algorithmic Complexity

| Module | Operation | Data Structure Used | Time Complexity | Space Complexity |
| :--- | :--- | :--- | :--- | :--- |
| **Auth** | Salt Generation | Array Indexing (`rand()`) | $O(S)$ *(S = salt length)* | $O(1)$ |
| **Auth** | Password Hashing | DJB2 Bit-Shifting Loop | $O(L)$ *(L = password length)* | $O(1)$ |
| **Auth** | User Lookup | Sequential Stream / Hash Map | $O(N)$ Disk / $O(1)$ RAM | $O(1)$ |
| **Tasks** | Add New Task | Doubly Linked List | **$O(1)$** | $O(1)$ |
| **Tasks** | Search Task | Hash Map (`unordered_map`) | **$O(1)$** | $O(1)$ |
| **Tasks** | Delete Task | Linked List Node Re-linking | **$O(1)$** | $O(1)$ |
| **Tasks** | Save Workspace | Output File Stream (`ofstream`)| $O(K)$ *(K = total tasks)* | $O(1)$ |

---

## 📂 Storage Schema & File Specifications

### 1. Master Authentication Registry (`login.txt`)
Stores login credentials using a pipe-delimited schema:
```text
username|salt|hashed_password
```
**Sample Records:**
```text
aavishkar|7x#A2|18446744073709551614
henry|9z$K1|12938491823901928312
john_doe|mK9!0|98234819238471928341
```

### 2. User-Specific Task Workspace (`<filename>.txt`)
Each user manages a personalized flat file storing their tasks:
```text
task_name|scheduled_time|scheduled_date|
```
**Sample Records (`aavishkar_tasks.txt`):**
```text
SolveLeetCode|18:00|31-10-2026|
GymWorkout|20:00|31-10-2026|
ReadDocumentation|22:30|31-10-2026|
```

---

## ⚙️ Compilation & Installation

### System Requirements
* **Operating System:** Windows (uses `<conio.h>` and `<windows.h>`)
* **Compiler:** MinGW / GCC, Clang, or MSVC supporting **C++11 or higher**

### Build via Command Prompt / PowerShell:
```bash
# 1. Clone the repository
git clone https://github.com/your-username/secure-cli-auth-task-manager.git

# 2. Navigate to the project directory
cd secure-cli-auth-task-manager

# 3. Compile the source code
g++ -std=c++11 main.cpp -o AuthTaskManager.exe

# 4. Launch the application
AuthTaskManager.exe
```

---

## 🖥️ Live Terminal Walkthrough

### Scenario A: New User Registration
```text
====================================================
                Auth System
====================================================
Enter your Username: aavishkar
Enter your Password: *****

Account not found. Creating a new account
Enter the name of the list: my_daily_tasks
Account created successfully

====================================================
Select 1 to add task.
Select 2 to delete task.
Select 3 to save and exit.
====================================================
Enter choice: 1
Enter the taskname.(Preferably keep it to one word action): Coding
Enter the time: 10:00AM
Enter the date: 01-11-2026

====================================================
Select 1 to add task.
Select 2 to delete task.
Select 3 to save and exit.
====================================================
Enter choice: 3
To do list saved successfully
```

### Scenario B: Returning User Login
```text
====================================================
                Auth System
====================================================
Enter your Username: aavishkar
Enter your Password: *****

User found
====================================================
                Welcome aavishkar!
====================================================

====================================================
Select 1 to add task.
Select 2 to delete task.
Select 3 to save and exit.
====================================================
Enter choice: 2
Enter the task name to be deleted: Coding
Task deleted successfully
```

---

## 🛡️ Security Design Principles Applied

1. **Principle of Least Privilege (PoLP):** Only the active user's credentials and file paths are loaded into execution memory during runtime.
2. **Defensive Programming:** Null-pointer checks around node re-linking prevent segmentation faults during edge-case deletions.
3. **Deterministic State Synchronization:** Tasks are cached in linked memory for high-speed manipulation and written to persistent storage upon explicit user exit commands.

---

## 📜 License
This project is open-source and available under the [MIT License](LICENSE).

## 👨‍💻 Author
* **Aavishkar** — Software Engineering & Systems Enthusiast
