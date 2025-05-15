# ✈️ Flight Reservation System in C++

This project is a command-line-based **Flight Reservation System** implemented in C++. It simulates real-world airline operations, including user registration, flight booking, seat allocation, and ticket management — using only custom data structures like:

- ✅ **Binary Search Tree (BST)** for managing airports and flights
- ✅ **Queue** for handling customer check-ins and offers
- ✅ **Doubly Linked List (DLL)** for maintaining dynamic ticket and flight records

---

## 🚀 Features

- 👤 Customer Registration & Login
- 📅 View Available Flights
- 🎫 Book, Cancel, and View Tickets
- 🛫 Manage Airports and Flights (Admin)
- 📋 Generate Receipts & Passenger Details
- ❌ No STL used (custom BST, Queue, DLL)

---

## 🧱 Data Structures Used

| Structure | Purpose |
|----------|---------|
| `BST<T>` | Storing flights & airports for efficient searching |
| `Queue<T>` | Managing customer queues & special offers |
| `DoublyLinkedList<T>` | Ticket lists and reservation history |

---

## 🛠️ How to Compile

```bash
g++ -o flight_reservation main.cpp
./flight_reservation
