# Secure Multi-User To-Do List System with CLI Authentication

A high-performance, multi-user To-Do List and Task Management application built in C++. This project combines a **cryptographically-salted DJB2 authentication gateway** with an **in-memory Doubly Linked List and Hash Map engine**, routing each authenticated user to their own dedicated task storage file.

---

## 📌 Project Overview

Traditional console-based To-Do lists either store all user tasks in a single shared file or keep passwords in plain text. This project solves both issues:
1. **User-Isolated Task Files:** On registration, each user creates or gets assigned a dedicated task file (`<list_name>.txt`). A user can never access or overwrite another user's task list.
2. **O(1) In-Memory Task Operations:** Tasks are maintained in a **Doubly Linked List** indexed by an `unordered_map<string, Node*>`, enabling instant $O(1)$ task insertion and $O(1)$ deletion by task name without linear array scanning.
3. **Salted DJB2 Authentication:** Prevents credential leaks and Rainbow Table attacks using a dynamic salt and one-way cryptographic hash per account.
4. **Interactive CLI Security:** Passwords are typed with real-time `*` masking using Windows `_getch()` with full Backspace and Enter support.

---

## 🏗️ System Architecture & Workflow

```
                        [ User Login / Registration ]
                                      │
                                      ▼
                         ┌─────────────────────────┐
                         │   Read user_login.txt   │
                         └────────────┬────────────┘
                                      │
                   ┌──────────────────┴──────────────────┐
                   ▼                                     ▼
            [ User Found ]                        [ New User ]
                   │                                     │
           Validate Password                     Prompt: List File Name
           via DJB2 Hash + Salt                  Generate Salt & Hash
                   │                                     │
                   │                             Save to user_login.txt
                   │                             Create <filename>.txt
                   │                                     │
                   └──────────────────┬──────────────────┘
                                      │
                                      ▼
                      [ Active User Workspace Opened ]
                      Assigned File: <filename>.txt
                                      │
                                      ▼
                  ┌──────────────────────────────────────┐
                  │      To-Do List Engine (Memory)      │
                  │  Doubly Linked List + Task Hash Map  │
                  └──────────────────┬───────────────────┘
                                     │
            ┌────────────────────────┼────────────────────────┐
            ▼                        ▼                        ▼
      [ 1. Add Task ]        [ 2. Delete Task ]       [ 3. Save & Exit ]
        O(1) Insert             O(1) Removal             File Serialization
       Appends to DLL        Unlinks DLL Node via      Dumps DLL to user's
      & Updates HashMap       HashMap Pointer           personal .txt file
```

---

## 🔑 Core Features

### 1. To-Do List & Task Management Engine
* **Constant Time Insertion (O(1)):** New tasks with task name, time, and date are attached directly to the tail of the Doubly Linked List in $O(1)$ time.
* **Instant Deletion by Name (O(1)):** An `unordered_map<string, Node*>` maps task names directly to their memory addresses in the linked list, allowing instant pointer re-linking without traversing the list.
* **Duplicate Task Prevention:** The hash map validates incoming task names in $O(1)$ time to reject duplicate entries.
* **Custom User Task Files:** Users define their own task list name upon sign-up, ensuring their tasks persist in an isolated file (e.g., `work_tasks.txt`, `daily_routines.txt`).
* **Structured File Persistence:** Upon choosing **Save & Exit**, all memory-resident tasks are serialized into the user's file using pipe delimiters (`|`).

### 2. Authentication & Access Control
* **Salted DJB2 Hashing:** Passwords are concatenated with a unique 5-character randomized salt and transformed into an integer digest using Daniel J. Bernstein's algorithm (`hash * 33 + c`).
* **Zero Memory Leakage:** Inactive users' data is never loaded into RAM. The system locates and verifies credentials on-demand from disk.
* **Keystroke Masking:** Uses `_getch()` to intercept console inputs, rendering asterisks (`*`) while protecting the actual input from shoulder surfing.
* **Case-Insensitive Usernames:** Sanitizes username comparisons to lower-case to prevent file-naming collisions on Windows file systems.

---

## 📊 Algorithmic Complexity

| Component | Operation | Data Structure Used | Time Complexity | Space Complexity |
| :--- | :--- | :--- | :--- | :--- |
| **Auth** | Password Hashing | DJB2 Loop | $O(L)$ *(L = password length)* | $O(1)$ |
| **Auth** | User Verification | File Stream Search | $O(N)$ Disk scan | $O(1)$ RAM |
| **To-Do** | Add Task | Doubly Linked List + Map | **$O(1)$** | $O(1)$ |
| **To-Do** | Delete Task by Name | Hash Map + Node Unlink | **$O(1)$** | $O(1)$ |
| **To-Do** | Duplicate Check | Hash Map Lookup | **$O(1)$** | $O(1)$ |
| **To-Do** | Save Tasks to Disk | Linked List Traversal | $O(K)$ *(K = total tasks)* | $O(1)$ |

---

## 📂 File Storage Format & Schema

### 1. Master User Registry (`user_login.txt`)
Stores login credentials and each user's mapped task file name using pipe (`|`) delimiters:
```text
username|salt|hashed_password|user_filename.txt
```

**Example:**
```text
peter|7x#A2|18446744073709551614|peter_tasks.txt
henry|9z$K1|12938491823901928312|henry_work.txt
alice|mK9!0|98234819238471928341|alice_study.txt
```

### 2. User-Specific Task File (`<filename>.txt`)
Stores individual task records scheduled by that user:
```text
task_name|scheduled_time|scheduled_date|
```

**Example (`peter_tasks.txt`):**
```text
SolveLeetCode|18:00|31-10-2026|
GymWorkout|20:00|31-10-2026|
ProjectMeeting|22:30|01-11-2026|
```

---

## ⚙️ Compilation & Setup

### Prerequisites
* **Operating System:** Windows (relies on `<conio.h>` for `_getch()` and `<windows.h>` for UTF-8 support)
* **Compiler:** MinGW / GCC, Clang, or MSVC supporting **C++11 or higher**

### Build Commands:
```bash
# 1. Clone this repository
git clone https://github.com/your-username/secure-cli-todo-system.git

# 2. Enter project folder
cd secure-cli-todo-system

# 3. Compile source code
g++ -std=c++11 main.cpp -o ToDoListApp.exe

# 4. Run application
ToDoListApp.exe
```

---

## 🖥️ Live Terminal Walkthrough

### 1. New Account Creation & List Setup
```text
====================================================
                To-Do List
====================================================
Enter your Username
peter
Enter your Password
*****

Account not found. Create a new account
Enter the name of the list:
my_tasks
Account created successfully

====================================================
Select 1 to add task.
Select 2 to delete task.
Select 3 to save and exit.
====================================================
1
Enter the taskname.(Preferably keep it to one word action).
Coding
Enter the time.
10:00AM
Enter the date.
01-11-2026

====================================================
Select 1 to add task.
Select 2 to delete task.
Select 3 to save and exit.
====================================================
3
To do list saved successfully
```

### 2. Returning User Login & Task Deletion
```text
====================================================
                To-Do List
====================================================
Enter your Username
peter
Enter your Password
*****

User found

====================================================
Select 1 to add task.
Select 2 to delete task.
Select 3 to save and exit.
====================================================
2
Enter the task name to be deleted.
Coding
Task deleted successfully

====================================================
Select 1 to add task.
Select 2 to delete task.
Select 3 to save and exit.
====================================================
3
To do list saved successfully
```

---

## 🛡️ Key Software Engineering Principles
* **Separation of Concerns (SoC):** Authentication logic (`Login` class) is decoupled from the task management business logic (`Todolist` class).
* **RAII & Resource Cleanup:** File streams are explicitly flushed and closed, and node pointers are re-linked safely to prevent memory corruption.
* **Defensive Edge-Case Checks:** Null-pointer checks ensure that deleting a non-existent task or attempting duplicate insertions produces clean error messages without application crashes.


## 👨‍💻 Author
* **Aavishkar**
