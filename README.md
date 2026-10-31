# 🛡️ Secure CLI Authentication & Task Management System

A high-performance, security-focused Command-Line Interface (CLI) application built in C++. This project pairs a **cryptographically-salted DJB2 authentication engine** with an **in-memory, user-isolated Task Management System** powered by a Doubly Linked List and Hash Map.

---

## 📌 Features

### 🔐 1. Authentication & Security Engine
* **Salted Password Hashing:** Uses the **DJB2 hashing algorithm** combined with randomly generated unique salts per user to neutralize dictionary and **Rainbow Table attacks**.
* **Zero Plain-Text Storage:** Passwords are never stored directly in memory or on disk; only one-way cryptographic hashes are persisted.
* **Masked Console Input:** Interactive terminal password field that intercepts keystrokes via `_getch()` to display `*` masks, complete with Backspace and newline handling.
* **Lazy User Caching:** Verifies credentials on-demand from disk without dumping the entire user database into RAM, mitigating memory dump vulnerabilities.

### 📝 2. Task Management Engine
* **User-Isolated Workspaces:** Each authenticated user is automatically routed to their own personal storage file (`username_todo.txt`), preventing cross-user data leaks.
* **$O(1)$ Task Operations:** Powered by a **Doubly Linked List** and an `unordered_map` index to achieve instant $O(1)$ insertions and deletions by task name.
* **Persistent Storage:** Tasks are structured and saved using delimiter-based file serialization (`|`), ensuring state preservation across sessions.

---

## 🏗️ System Architecture
