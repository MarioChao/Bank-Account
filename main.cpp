#include <iostream>
#include <vector>
#include <limits>
#include "inc/bankAccount.h"
#include "inc/bankSystem.h"
#include "inc/cinFunctions.h"
using std::cin;
using std::cout;

void loadSavedInfo();
void testProgram();
void createAccount();
void accessAccount();
void deposit();
void withdraw();
void transfer();
BankAccount *yourAccount = nullptr;

int main() {
    loadSavedInfo();
    testProgram();
}

/**
 * @brief Load the saved bank accounts into the bank system
 * 
 */
void loadSavedInfo() {
    BankSystem::retrieveAccounts();
}

/**
 * @brief Test out the program!
 * 
 */
void testProgram() {
    while (true) {
        cout << "Command list: create, list, access, deposit, withdraw, transfer,\n";
        cout << "\t show number, show info, show history, log out, delete\n";
        cout << "Enter a command:\n";
        string cmd;
        cout << ">";
        getline(cin >> std::ws, cmd);

        if (cmd == "create") {
            createAccount();
        } else if (cmd == "list") {
            BankSystem::listAccountNumbers();
        } else if (cmd == "access") {
            accessAccount();
        } else if (cmd == "deposit") {
            deposit();
        } else if (cmd == "withdraw") {
            withdraw();
        } else if (cmd == "transfer") {
            transfer();
        } else if (cmd == "show number") {
            if (yourAccount == nullptr) cout << "No account selected.\n";
            else yourAccount -> showAccountNumber();
        } else if (cmd == "show info") {
            if (yourAccount == nullptr) cout << "No account selected.\n";
            else yourAccount -> showInfo();
        } else if (cmd == "show history") {
            if (yourAccount == nullptr) cout << "No account selected.\n";
            else yourAccount -> showTransactions();
        } else if (cmd == "log out") {
            yourAccount = nullptr;
            cout << "Logged out\n";
        } else if (cmd == "delete") {
            if (yourAccount == nullptr) cout << "No account selected.\n";
            else BankSystem::removeAccount(yourAccount);
        }
        cout << '\n';
    }
}

void createAccount() {
    cout << "Enter account type:\n";
    int type;
    cout << ">";
    cin >> std::ws >> type;
    while (!cin) {
        clearCin();
        cout << ">";
        cin >> std::ws >> type;
    }

    cout << "Enter account balance:\n";
    double bal;
    cout << ">";
    cin >> std::ws >> bal;
    while (!cin) {
        clearCin();
        cout << ">";
        cin >> std::ws >> bal;
    }

    delete yourAccount;
    yourAccount = new BankAccount(type, bal);
    cout << "Account " << yourAccount -> getAccountNumber() << " created!\n";
}

void accessAccount() {
    string number;
    cout << "Enter account number:\n";
    cout << ">";
    cin >> std::ws >> number;
    while (!cin) {
        clearCin();
        cout << ">";
        cin >> std::ws >> number;
    }
    if (yourAccount = BankSystem::authenticate(number)) {
        cout << "Account " << number << " authenticated.\n";
    } else {
        cout << "Accessing account failed!\n";
    }
}

void deposit() {
    if (yourAccount == nullptr) {
        cout << "No account selected.\n";
        return;
    }
    cout << "Enter amount to deposit:\n";
    double amt;
    cout << ">";
    cin >> std::ws >> amt;
    while (!cin) {
        clearCin();
        cout << ">";
        cin >> std::ws >> amt;
    }
    if (yourAccount -> deposit(amt)) {
        cout << "Deposited " << amt << " to your bank account.\n";
    } else {
        cout << "Error in deposit!\n";
    }
}

void withdraw() {
    if (yourAccount == nullptr) {
        cout << "No account selected.\n";
        return;
    }
    cout << "Enter amount to withdraw:\n";
    double amt;
    cout << ">";
    cin >> std::ws >> amt;
    while (!cin) {
        clearCin();
        cout << ">";
        cin >> std::ws >> amt;
    }
    if (yourAccount -> withdraw(amt)) {
        cout << "Withdrawn " << amt << " from your bank account.\n";
    } else {
        cout << "Error in withdraw!\n";
    }
}

void transfer() {
    if (yourAccount == nullptr) {
        cout << "No account selected.\n";
        return;
    }
    cout << "Enter bank account number to transfer to:\n";
    string number;
    cout << ">";
    cin >> std::ws >> number;
    while (!cin) {
        clearCin();
        cout << ">";
        cin >> std::ws >> number;
    }

    cout << "Enter amount to:\n";
    double amt;
    cout << ">";
    cin >> std::ws >> amt;
    while (!cin) {
        clearCin();
        cout << ">";
        cin >> std::ws >> amt;
    }
    if (yourAccount -> transferTo(number, amt)) {
        cout << "Transferred " << amt << " to your account " << number << ".\n";
    } else {
        cout << "Error in transfer!\n";
    }
}