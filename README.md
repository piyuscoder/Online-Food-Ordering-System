

# 🍴 Online Food Ordering System (C++ Console Application)

This is a robust, object-oriented **C++ console application** that simulates a functional food ordering and menu management system. It demonstrates mastery of **core C++ concepts**, advanced **STL containers**, and **efficient data handling techniques**, making it ideal for real-world scenarios.

---

## ✨ Key Features

* **🧩 Object-Oriented Design (OOP):** Modular, class-based architecture with dedicated classes for `Order`, `MenuItem`, and `FoodOrderingSystem`, ensuring clean code and maintainability.
* **⏱️ High-Performance Data Structures:** Utilizes `std::map` for fast menu item lookups, providing **O(log N)** search efficiency during order processing.
* **💾 Data Persistence (File I/O):** Loads the menu from an external `menu_data.txt` file on startup and saves changes dynamically, preserving data across sessions.
* **🔑 Administrator Mode:** Secure admin panel allows runtime updates, including adding new items or updating prices, without recompiling the program.
* **✅ Application Robustness:** Implements rigorous input validation for all user interactions, ensuring system stability and preventing errors.
* **🛒 Dynamic Order Management:** Supports calculating totals, modifying orders, and removing specific quantities of items in real-time.

---

## 💻 Technology Stack

* **Primary Language:** C++ (Standard C++11/14)
* **Libraries:** Standard Template Library (STL)
* **Data Structures:** `std::map`, `std::vector`, `std::stringstream`
* **Environment:** Console/Terminal Application

---

## 🛠️ How to Build & Run

### Prerequisites

* A C++ compiler installed (e.g., **G++ via MinGW on Windows** or **GCC on Linux**)

### Compilation

1. Navigate to the directory containing `online_food_system.cpp`
2. Compile using:

```bash
g++ online_food_system.cpp -o food_order -std=c++11
```

### Running the Application

1. Ensure `menu_data.txt` is present in the same directory as the executable.
2. Execute the program:

```bash
./food_order
```

3. The application launches a main menu, allowing users to place orders or access **Admin Menu** (option 6) for managing menu data.


