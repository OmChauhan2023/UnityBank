#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
using namespace std;

// Customer Class
class Customer {
    int customerID;
    string name;
    string address;
    string phoneNumber;

public:
    Customer(int id, string n, string a, string p) : customerID(id), name(n), address(a), phoneNumber(p) {}

    void modifyCustomer() {
        char c;
        cout << "\nCURRENT CUSTOMER DETAILS\n";
        cout << "Customer ID: " << customerID << endl;
        cout << "Name: " << name << endl;
        cout << "Address: " << address << endl;
        cout << "Phone Number: " << phoneNumber << endl;
        cout << "Would you like to modify the details? (y/n): ";
        cin >> c;

        if (c == 'y') {
            int choice;
            cout << "1. Name\n2. Address\n3. Phone Number\nEnter your choice (1-3): ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1:
                    cout << "Enter New Name: ";
                    getline(cin, name);
                    break;
                case 2:
                    cout << "Enter new address: ";
                    getline(cin, address);
                    break;
                case 3:
                    cout << "Enter new phone number: ";
                    getline(cin, phoneNumber);  
                    break;
                default:
                    cout << "Invalid choice.\n";
                    break;
            }
            cout << "Customer details updated successfully!\n";
        } else {
            cout << "No modifications made.\n";
        }
    }

    void displayCustomer() const {
        cout << "Customer ID: " << customerID << endl;
        cout << "Name: " << name << endl;
        cout << "Address: " << address << endl;
        cout << "Phone Number: " << phoneNumber << endl;
    }

    int getCustomerID() const { return customerID; }
    string getName() const { return name; }
    string getAddress() const { return address; }
    string getPhoneNumber() const { return phoneNumber; }
};

// Base Account Class
class Account {
protected:
    int accountNumber;
    float balance;
    Customer* customer;

public:
    Account(int a, float b, Customer* cust) : accountNumber(a), balance(b), customer(cust) {}

    virtual void deposit(float amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposit successful! New balance: " << balance << endl;
        } else {
            cout << "Invalid deposit amount!\n";
        }
    }

    virtual void withdraw(float amount) {
        if (amount <= 0) {
            cout << "Invalid withdraw amount!\n";
        } else if (amount > balance) {
            cout << "Insufficient balance!\n";
        } else {
            balance -= amount;
            cout << "Withdrawal successful! New balance: " << balance << endl;
        }
    }

    virtual void displayAccountDetails() const {
        cout << "\nACCOUNT DETAILS\n";
        cout << "Account Number: " << accountNumber << endl;
        cout << "Balance: " << balance << endl;
        customer->displayCustomer();
    }

    Customer* getCustomer() const {
        return customer;
    }

    int getAccountNumber() const {
        return accountNumber;
    }

    float getBalance() const {
        return balance;
    }
};

// Savings Account Class
class SavingsAccount : public Account {
    float interestRate;

public:
    SavingsAccount(int a, float b, Customer* cust, float rate) : Account(a, b, cust), interestRate(rate) {}

    void calculateInterest() {
        float interest = balance * interestRate / 100;
        balance += interest;
        cout << "Interest added: " << interest << endl;
        cout << "New Balance after interest: " << balance << endl;
    }

    void displayAccountDetails() const override {
        Account::displayAccountDetails();
        cout << "Interest Rate: " << interestRate << "%" << endl;
    }
};

// Current Account Class
class CurrentAccount : public Account {
    double overdraftLimit;

public:
    CurrentAccount(int a, float b, Customer* cust, double limit) : Account(a, b, cust), overdraftLimit(limit) {}

    void withdraw(float amount) override {
        if (amount <= balance + overdraftLimit) {
            balance -= amount;
            cout << "Withdrawal successful! New balance: " << balance << endl;
        } else {
            cout << "Withdrawal exceeds overdraft limit!\n";
        }
    }

    void displayAccountDetails() const override {
        Account::displayAccountDetails();
        cout << "Overdraft Limit: " << overdraftLimit << endl;
    }
};

// Bank Class
class Bank {
    vector<Account*> accounts;
    vector<Customer*> customers;

public:
    void createAccount(Account* account) {
        accounts.push_back(account);
        cout << "Account created successfully.\n";
    }

    void createCustomer(Customer* customer) {
        customers.push_back(customer);
        cout << "Customer created successfully.\n";
    }

    void deleteAccount(int accountNumber) {
        for (auto it = accounts.begin(); it != accounts.end(); ++it) {
            if ((*it)->getAccountNumber() == accountNumber) {
                accounts.erase(it);
                cout << "Account " << accountNumber << " deleted.\n";
                return;
            }
        }
        cout << "Account not found.\n";
    }

    void displayCustomerAccounts() const {
        for (auto& account : accounts) {
            account->displayAccountDetails();
        }
    }

    void executeTransaction(int accountNumber, float amount, const string& type) {
        for (auto& account : accounts) {
            if (account->getAccountNumber() == accountNumber) {
                if (type == "Deposit") {
                    account->deposit(amount);
                } else if (type == "Withdraw") {
                    account->withdraw(amount);
                }
                break;
            }
        }
    }

    vector<Customer*>& getCustomers() {
        return customers;
    }

    const vector<Account*>& getAccounts() const {
        return accounts;
    }

    // Save customers and accounts to a file
    void saveData() {
        ofstream customerFile("customers.txt");
        ofstream accountFile("accounts.txt");

        if (customerFile.is_open() && accountFile.is_open()) {
            // Save customers
            for (const auto& customer : customers) {
                customerFile << customer->getCustomerID() << "\n";
                customerFile << customer->getName() << "\n";
                customerFile << customer->getAddress() << "\n";
                customerFile << customer->getPhoneNumber() << "\n";
            }

            // Save accounts
            for (const auto& account : accounts) {
                accountFile << account->getAccountNumber() << "\n";
                accountFile << account->getBalance() << "\n";
                accountFile << account->getCustomer()->getCustomerID() << "\n"; // Link account to customer
            }

            customerFile.close();
            accountFile.close();
        } else {
            cout << "Error saving data to file.\n";
        }
    }

    // Load customers and accounts from a file
    void loadData() {
        ifstream customerFile("customers.txt");
        ifstream accountFile("accounts.txt");

        if (customerFile.is_open() && accountFile.is_open()) {
            int id;
            string name, address, phone;

            // Load customers
            while (customerFile >> id) {
                customerFile.ignore(); 
                getline(customerFile, name);
                getline(customerFile, address);
                getline(customerFile, phone);

                Customer* newCustomer = new Customer(id, name, address, phone);
                customers.push_back(newCustomer);
            }

            int accountNumber;
            float balance;
            int customerID;

            // Load accounts
            while (accountFile >> accountNumber >> balance >> customerID) {
                Customer* customer = nullptr;
                for (const auto& c : customers) {
                    if (c->getCustomerID() == customerID) {
                        customer = c;
                        break;
                    }
                }

                if (customer) {
                    Account* newAccount = new CurrentAccount(accountNumber, balance, customer, 0);
                    accounts.push_back(newAccount);
                }
            }

            customerFile.close();
            accountFile.close();
        } else {
            cout << "Error loading data from file.\n";
        }
    }
};

// Admin Class
class Admin {
    string username = "admin";
    string password = "3456";

public:
    Admin(string u, string p) : username(u), password(p) {}

    bool authenticate(string u, string p) {
        return (u == username && p == password);
    }

    void viewAllAccounts(const Bank& bank) {
        bank.displayCustomerAccounts();
    }

    void createNewCustomer(Bank& bank) {
        int id;
        string name, address, phone;

        cout << "\nEnter customer details:\n";
        cout << "Customer ID: ";
        cin >> id;
        cin.ignore(); 
        cout << "Customer Name: ";
        getline(cin, name);
        cout << "Customer Address: ";
        getline(cin, address);
        cout << "Customer Phone Number: ";
        getline(cin, phone);

        Customer* newCustomer = new Customer(id, name, address, phone);
        bank.createCustomer(newCustomer);

        // Optionally, create an account for the new customer
        cout << "\nWould you like to create an account for this customer? (y/n): ";
        char createAccountChoice;
        cin >> createAccountChoice;
        if (createAccountChoice == 'y') {
            int accountChoice;
            cout << "Select account type:\n1. Savings Account\n2. Current Account\n";
            cin >> accountChoice;
            float balance;
            cout << "Enter initial balance: ";
            cin >> balance;

            if (accountChoice == 1) {
                float interestRate;
                cout << "Enter interest rate: ";
                cin >> interestRate;
                SavingsAccount* sa = new SavingsAccount(id, balance, newCustomer, interestRate);
                bank.createAccount(sa);
            } else if (accountChoice == 2) {
                float overdraftLimit;
                cout << "Enter overdraft limit: ";
                cin >> overdraftLimit;
                CurrentAccount* ca = new CurrentAccount(id, balance, newCustomer, overdraftLimit);
                bank.createAccount(ca);
            }
        }
    }

    void createNewAdmin() {
        string newUsername, newPassword;
        cout << "\nEnter new admin username: ";
        cin >> newUsername;
        cout << "Enter new admin password: ";
        cin >> newPassword;

        cout << "New admin created successfully!\n";
    }

    void closeAccount(Bank& bank) {
        int accountNumber;
        cout << "Enter the account number to close: ";
        cin >> accountNumber;
        bank.deleteAccount(accountNumber);
    }

    void modifyAccount(Bank& bank) {
        int accountNumber;
        cout << "Enter the account number to modify: ";
        cin >> accountNumber;

        string choice;
        cout << "Enter 'Deposit' or 'Withdraw' to modify the account: ";
        cin >> choice;

        float amount;
        cout << "Enter the amount: ";
        cin >> amount;
        bank.executeTransaction(accountNumber, amount, choice);
    }

    // Generate the report of all customers and their account details and save to a text file
    void generateCustomerReport(const Bank& bank) {
        cout << "Generating Customer Report...\n";

        ofstream reportFile("CustomerReport.txt");
    
        if (reportFile.is_open()) {
            reportFile << "------- Customer Report -------\n\n";
            
            for (const auto& account : bank.getAccounts()) {
                account->displayAccountDetails(); 
                reportFile << "Account Number: " << account->getAccountNumber() << "\n";
                reportFile << "Balance: " << account->getBalance() << "\n";
                reportFile << "Customer ID: " << account->getCustomer()->getCustomerID() << "\n";
                reportFile << "Customer Name: " << account->getCustomer()->getName() << "\n";
                reportFile << "Customer Address: " << account->getCustomer()->getAddress() << "\n";
                reportFile << "Customer Phone: " << account->getCustomer()->getPhoneNumber() << "\n";
                reportFile << "-----------------------------\n\n";
            }
            
            reportFile.close();
            cout << "Report generated successfully! Check CustomerReport.txt for details.\n";
        }
        else {
            cout << "Unable to open file to write report.\n";
        }
    }
};

// Menu Display Function for Admin
void displayAdminMenu() {
    cout << "\n--- Bank Admin Menu ---\n";
    cout << "1. View All Accounts\n";
    cout << "2. Close Account\n";
    cout << "3. Modify Account\n";
    cout << "4. Generate Customer Report\n";  
    cout << "5. Create New Customer\n";  
    cout << "6. Create New Admin\n";     
    cout << "7. Exit\n";
    cout << "Enter your choice: ";
}

// Menu Display Function for Customer
void displayCustomerMenu() {
    cout << "\n--- Customer Menu ---\n";
    cout << "1. Deposit\n";
    cout << "2. Withdraw\n";
    cout << "3. View Account Details\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    Admin admin("admin", "3456"); 
    Bank bank;  

    // Load existing customers and accounts from files (if available)
    bank.loadData();

    int loginChoice;

    cout << "\n --- Login System --------";
    cout << "\n  1. Customer Login       ";
    cout << "\n  2. Administrator Login  ";
    cout << "\n  Enter Your Choice: ";
    cin >> loginChoice;

    if (loginChoice == 1) {  
        int customerChoice;
        do {
            cout << "\n--- Customer Login ---\n";
            cout << "Enter Customer ID: ";
            int customerID;
            cin >> customerID;

            // Find the customer and their account
            Account* customerAccount = nullptr;
            for (auto& account : bank.getAccounts()) {
                if (account->getAccountNumber() == customerID) {
                    customerAccount = account;
                    break;
                }
            }

            if (customerAccount) {
                do {
                    // Display customer menu
                    displayCustomerMenu();
                    cin >> customerChoice;

                    float amount;
                    switch (customerChoice) {
                        case 1:  // Deposit
                            cout << "Enter the deposit amount: ";
                            cin >> amount;
                            customerAccount->deposit(amount);
                            break;

                        case 2:  // Withdraw
                            cout << "Enter the withdrawal amount: ";
                            cin >> amount;
                            customerAccount->withdraw(amount);
                            break;

                        case 3:  // View Account Details
                            customerAccount->displayAccountDetails();
                            break;

                        case 4:  // Exit
                            cout << "Exiting Customer Menu.\n";
                            break;

                        default:
                            cout << "Invalid choice. Please try again.\n";
                            break;
                    }
                } while (customerChoice != 4);  // Repeat until the customer chooses to exit

            } else {
                cout << "Customer ID not found! Please try again.\n";
            }

        } while (true);  // Keep prompting for customer login until valid credentials are provided
    } 
    else if (loginChoice == 2) {  // Admin Login
        // Admin Authentication
        string username, password;
        cout << "\nEnter admin username: ";
        cin >> username;
        cout << "Enter admin password: ";
        cin >> password;

        // Authenticate the admin
        if (admin.authenticate(username, password)) {
            cout << "Authentication successful.\n";
            int choice;
            do {
                // Display the admin menu
                displayAdminMenu();
                cin >> choice;

                switch (choice) {
                    case 1:
                        admin.viewAllAccounts(bank);  // View all accounts
                        break;
                    case 2:
                        admin.closeAccount(bank);  // Close an account
                        break;
                    case 3:
                        admin.modifyAccount(bank);  // Modify an account (deposit or withdraw)
                        break;
                    case 4:
                        admin.generateCustomerReport(bank);  // Generate a report of all accounts
                        break;
                    case 5:
                        admin.createNewCustomer(bank);  // Create a new customer
                        break;
                    case 6:
                        admin.createNewAdmin();  // Create a new admin
                        break;
                    case 7:
                        cout << "Exiting Admin Panel.\n";  // Exit the admin panel
                        break;
                    default:
                        cout << "Invalid choice.\n";
                }
            } while (choice != 7);  // Repeat until the user chooses to exit
        } else {
            cout << "Authentication failed.\n";
        }
    } else {
        cout << "Invalid choice.\n";
    }

    // Save data before exit
    bank.saveData();

    // Customer c(109,"Jatin", "Surat", "120849539");
    // c.modifyCustomer();
    // c.displayCustomer();
    
    return 0;
}
