

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <limits> // For numeric_limits

// --- 1. Data Structure: MenuItem ---
// Represents a single item on the menu.
struct MenuItem {
    int id;
    std::string name;
    double price;
};

// --- 2. Class: Order ---
// Manages the items currently selected by the customer.
class Order {
private:
    // Stores map of MenuItem ID to Quantity
    std::map<int, int> items; 
    double total_cost = 0.0;

public:
    // Adds an item to the current order
    void addItem(int item_id, int quantity, double item_price) {
        if (quantity > 0) {
            items[item_id] += quantity;
            total_cost += (item_price * quantity);
        }
    }

    // Removes a specific quantity of an item, or the item entirely.
    bool removeItem(int item_id, int quantity, double item_price) {
        if (items.count(item_id)) {
            if (quantity >= items[item_id]) {
                // Remove all quantity and update total cost
                total_cost -= (item_price * items[item_id]);
                items.erase(item_id);
                return true; // Item fully removed
            } else if (quantity > 0) {
                // Remove partial quantity
                items[item_id] -= quantity;
                total_cost -= (item_price * quantity);
                return true; // Partial quantity removed
            }
        }
        return false; // Item not found or invalid quantity
    }

    // Displays the current items in the order
    void displayOrder(const std::map<int, MenuItem>& menu_map) const {
        if (items.empty()) {
            std::cout << "\n    --> Order is currently empty. <--" << std::endl;
            return;
        }

        std::cout << "\n============================================\n";
        std::cout << "               YOUR ORDER\n";
        std::cout << "============================================\n";
        std::cout << std::left << std::setw(5) << "QTY"
                  << std::left << std::setw(25) << "ITEM"
                  << std::right << std::setw(10) << "PRICE" << std::endl;
        std::cout << "--------------------------------------------\n";
        
        for (const auto& pair : items) {
            int id = pair.first;
            int qty = pair.second;
            // Check if the item exists in the main menu map before displaying
            if (menu_map.count(id)) {
                const MenuItem& item = menu_map.at(id);
                std::cout << std::left << std::setw(5) << qty
                          << std::left << std::setw(25) << item.name
                          << std::right << std::setw(10) << std::fixed << std::setprecision(2) << (item.price * qty) << std::endl;
            }
        }
        std::cout << "--------------------------------------------\n";
        std::cout << std::left << std::setw(30) << "Total Amount Due:"
                  << std::right << std::setw(10) << std::fixed << std::setprecision(2) << total_cost << std::endl;
        std::cout << "============================================\n";
    }

    // Retrieves the total cost of the order
    double getTotal() const {
        return total_cost;
    }
    
    // Check if a specific item ID exists in the order
    bool hasItem(int item_id) const {
        return items.count(item_id) > 0;
    }

    // Clears the order after payment
    void clear() {
        items.clear();
        total_cost = 0.0;
    }
};

// --- 3. Class: FoodOrderingSystem ---
// The main application logic class, handling menu and operations.
class FoodOrderingSystem {
private:
    std::vector<MenuItem> menu_list; // For ordered display
    std::map<int, MenuItem> menu_map; // For quick lookup by ID
    Order current_order;
    const std::string menu_filename = "menu_data.txt"; // Constant file name
    int next_available_id = 1; // Tracks the next unique ID for new items

    // --- Helper function to display current menu ---
    void displayMenu() const {
        std::cout << "\n\n============================================\n";
        std::cout << "             TODAY'S MENU\n";
        std::cout << "============================================\n";
        std::cout << std::left << std::setw(5) << "ID"
                  << std::left << std::setw(25) << "ITEM"
                  << std::right << std::setw(10) << "PRICE" << std::endl;
        std::cout << "--------------------------------------------\n";

        for (const auto& item : menu_list) {
            std::cout << std::left << std::setw(5) << item.id
                      << std::left << std::setw(25) << item.name
                      << std::right << std::setw(10) << std::fixed << std::setprecision(2) << item.price << std::endl;
        }
        std::cout << "============================================\n";
    }

    // --- Core function to add items to the order ---
    void takeOrder() {
        int item_id;
        int quantity;
        std::cout << "\n--- Place Order ---\n";

        while (true) {
            displayMenu();
            std::cout << "Enter Item ID to order (0 to finish): ";
            std::cin >> item_id;

            if (item_id == 0) {
                break;
            }

            // Input validation for item_id
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number for Item ID.\n";
                continue;
            }

            // Check if item ID exists using the map for efficient lookup
            if (menu_map.count(item_id)) {
                std::cout << "Enter Quantity for " << menu_map.at(item_id).name << ": ";
                std::cin >> quantity;

                if (std::cin.fail() || quantity <= 0) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid quantity. Please try again.\n";
                    continue;
                }

                const MenuItem& item = menu_map.at(item_id);
                current_order.addItem(item.id, quantity, item.price);
                std::cout << quantity << " x " << item.name << " added to order.\n";
            } else {
                std::cout << "Invalid Item ID. Please choose from the menu.\n";
            }
        }
    }
    
    // Function to remove items from the current order
    void modifyOrder() {
        if (current_order.getTotal() == 0.0) {
            std::cout << "\nOrder is empty. Nothing to modify.\n";
            return;
        }

        current_order.displayOrder(menu_map);
        std::cout << "\n--- Modify Order ---\n";
        int item_id, quantity;

        std::cout << "Enter Item ID to remove (0 to cancel): ";
        std::cin >> item_id;

        if (item_id == 0) return;

        if (current_order.hasItem(item_id)) {
            std::cout << "Enter quantity to remove: ";
            std::cin >> quantity;
            
            if (std::cin.fail() || quantity <= 0) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid quantity.\n";
                return;
            }
            
            const MenuItem& item = menu_map.at(item_id);
            if (current_order.removeItem(item_id, quantity, item.price)) {
                std::cout << quantity << " x " << item.name << " successfully removed.\n";
            } else {
                std::cout << "Error removing item. Check quantity or ID.\n";
            }
        } else {
            std::cout << "Error: Item ID not found in your current order.\n";
        }
        // Clear stream buffer
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    // --- Finalizes the order and handles payment ---
    void processPayment() {
        if (current_order.getTotal() == 0.0) {
            std::cout << "\nOrder is empty. Nothing to pay.\n";
            return;
        }

        double total = current_order.getTotal();
        std::cout << "\n*** PROCESSING PAYMENT ***\n";
        current_order.displayOrder(menu_map);

        double amount_paid = 0.0;
        std::cout << "\nTotal Due: $" << std::fixed << std::setprecision(2) << total << std::endl;
        
        while (amount_paid < total) {
            std::cout << "Enter amount to pay: $";
            std::cin >> amount_paid;

            if (std::cin.fail() || amount_paid < total) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Insufficient amount or invalid input. Please try again.\n";
                amount_paid = 0.0; // Reset for re-entry
            }
        }
        
        std::cout << "\n--- Transaction Complete ---\n";
        std::cout << "Change Due: $" << std::fixed << std::setprecision(2) << (amount_paid - total) << std::endl;
        std::cout << "Thank you for your order!\n";
        current_order.clear(); // Reset order for next customer
    }

    // --- Admin function to save menu to file ---
    bool saveMenuToFile() const {
        std::ofstream file(menu_filename);
        if (!file.is_open()) {
            std::cout << "Error: Could not open file to save menu.\n";
            return false;
        }

        // Write menu items back to file in the specified format: Name,Price
        for (const auto& item : menu_list) {
            file << item.name << "," << std::fixed << std::setprecision(2) << item.price << "\n";
        }
        file.close();
        std::cout << "\n--- Admin Action: Menu saved successfully to " << menu_filename << ".\n";
        return true;
    }

    // --- NEW ADMIN FUNCTIONS ---

    // Adds a new item to the menu in runtime
    void addNewItem() {
        std::string name;
        double price;

        std::cout << "\n--- Admin: Add New Item ---\n";
        std::cout << "Enter Item Name: ";
        // Clear cin buffer and get line to allow spaces in name
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, name);

        std::cout << "Enter Price: $";
        std::cin >> price;

        if (std::cin.fail() || price <= 0) {
            std::cout << "Invalid price. Item not added.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }

        // Assign next available ID
        MenuItem new_item = {next_available_id, name, price};
        menu_list.push_back(new_item);
        menu_map[next_available_id] = new_item;
        
        std::cout << "Item added: ID " << next_available_id << " - " << name << " ($" << price << ")\n";
        next_available_id++; // Increment ID for the next item
    }

    // Updates the price of an existing menu item
    void updateItemPrice() {
        int id;
        double new_price;

        std::cout << "\n--- Admin: Update Price ---\n";
        displayMenu();
        std::cout << "Enter Item ID to update: ";
        std::cin >> id;

        if (std::cin.fail()) {
            std::cout << "Invalid ID input.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }

        // Use map for fast lookup
        if (menu_map.count(id)) {
            std::cout << "Current Price for " << menu_map.at(id).name << ": $" << menu_map.at(id).price << std::endl;
            std::cout << "Enter NEW Price: $";
            std::cin >> new_price;

            if (std::cin.fail() || new_price <= 0) {
                std::cout << "Invalid price. Price update cancelled.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return;
            }

            // Update in map (for fast access)
            menu_map.at(id).price = new_price; 
            
            // Also update in list (for sequential display)
            for (auto& item : menu_list) {
                if (item.id == id) {
                    item.price = new_price;
                    break;
                }
            }

            std::cout << "Price for ID " << id << " updated to $" << new_price << std::endl;
        } else {
            std::cout << "Error: Item ID " << id << " not found.\n";
        }
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    // The main admin sub-menu
    void adminMenu() {
        int admin_choice;
        do {
            std::cout << "\n\n=== ADMIN PANEL ===\n";
            std::cout << "1. Add New Item\n";
            std::cout << "2. Update Item Price\n";
            std::cout << "3. Save Menu Changes to File (REQUIRED to save permanently)\n";
            std::cout << "4. Go Back to Main Menu\n";
            std::cout << "Enter choice: ";
            std::cin >> admin_choice;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number.\n";
                admin_choice = 0; 
                continue;
            }

            switch (admin_choice) {
                case 1: addNewItem(); break;
                case 2: updateItemPrice(); break;
                case 3: saveMenuToFile(); break;
                case 4: std::cout << "Exiting Admin Panel.\n"; break;
                default: std::cout << "Invalid choice.\n";
            }
        } while (admin_choice != 4);
    }

public:
    FoodOrderingSystem() {
        // Load initial menu when system is created
        if (!loadMenuFromFile(menu_filename)) {
            // If file loading fails, populate with default items
            std::cout << "Note: Could not load menu file. Using default menu.\n";
            MenuItem defaults[] = {
                {1, "Veggie Burger", 5.99},
                {2, "Cheese Pizza Slice", 3.50},
                {3, "French Fries (Large)", 2.99},
                {4, "Soda (Coke/Pepsi)", 1.50},
                {5, "Bottled Water", 1.00},
                {6, "Iced Coffee", 4.00}
            };
            for (const auto& item : defaults) {
                menu_list.push_back(item);
                menu_map[item.id] = item;
            }
            // Set next ID higher than the largest default ID (6)
            next_available_id = 7; 
        } else {
             // Find the highest ID loaded from file and set next_available_id
            for(const auto& item : menu_list) {
                if(item.id >= next_available_id) {
                    next_available_id = item.id + 1;
                }
            }
        }
    }

    // --- Function to load menu from a file (demonstrates file I/O) ---
    bool loadMenuFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            // File doesn't exist or cannot be opened
            return false;
        }

        menu_list.clear();
        menu_map.clear();
        std::string line;
        int current_id = 1; // Start ID counter for menu loading

        // File format assumed: Name, Price
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string name_part;
            double price_part;
            
            // Assuming name is until the last comma, and price is after it
            size_t comma_pos = line.rfind(',');
            if (comma_pos != std::string::npos) {
                name_part = line.substr(0, comma_pos);
                std::string price_str = line.substr(comma_pos + 1);
                
                std::stringstream price_ss(price_str);
                if (price_ss >> price_part) {
                     // Use sequential ID based on line number
                     menu_list.push_back({current_id, name_part, price_part});
                     menu_map[current_id] = menu_list.back();
                     current_id++;
                }
            }
        }
        file.close();
        
        // Update the next_available_id after loading
        next_available_id = current_id;

        if (menu_list.empty()) {
            return false; // File was empty
        }
        std::cout << "Menu loaded successfully from " << filename << ".\n";
        return true;
    }

    // --- Main loop to run the system ---
    void run() {
        int choice;
        do {
            std::cout << "\n\n--- Food Ordering System ---\n";
            std::cout << "1. View Menu\n";
            std::cout << "2. Place New Order\n";
            std::cout << "3. View Current Order\n";
            std::cout << "4. Modify Current Order (Remove Item)\n"; 
            std::cout << "5. Proceed to Payment\n";
            std::cout << "6. Enter Admin Panel\n";         // UPDATED OPTION
            std::cout << "7. Exit System\n";
            std::cout << "Enter choice: ";
            std::cin >> choice;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number.\n";
                choice = 0; // Force loop continuation
                continue;
            }

            switch (choice) {
                case 1: displayMenu(); break;
                case 2: takeOrder(); break;
                case 3: current_order.displayOrder(menu_map); break;
                case 4: modifyOrder(); break;           
                case 5: processPayment(); break;
                case 6: adminMenu(); break;        // Call the new Admin Menu
                case 7: std::cout << "Exiting. Goodbye!\n"; break;
                default: std::cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 7);
    }
};

int main() {
    // Set up standard output format
    std::cout << std::fixed << std::setprecision(2);
    FoodOrderingSystem system;
    system.run();
    return 0;
}
