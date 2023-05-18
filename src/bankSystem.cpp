#include "../inc/bankSystem.h"
#include "../inc/bankAccount.h"
#include "../inc/cinFunctions.h"

// Static declaration

vector<BankAccount> BankSystem::accountList;

// Public field

/**
 * @brief Add the bank account to the account list in the bank system
 * 
 * @param account The bank account to be added
 */
void BankSystem::addAccountToList(BankAccount *account) {
    if (account == nullptr) return;
    if (getAccountIndex(account -> getAccountNumber()) >= 0) return;

    accountList.push_back(*account);
    saveAccountNumber(account -> getAccountNumber());
}

/**
 * @brief Remove all info about a bank account in the bank system
 * 
 * @param account The account to be removed
 */
void BankSystem::removeAccount(BankAccount *account) {
    if (account == nullptr) return;
    int id =  getAccountIndex(account -> getAccountNumber());
    if (id == -1) return;

    // Confirmation
    cout << "Are you sure? [Y|N]\n";
    string resp;
    cout << ">";
    cin >> resp;
    while (!cin || string("YN").find(resp[0]) == string::npos) {
        clearCin();
        cout << ">";
        cin >> resp;
    }
    if (resp[0] == 'N') {
        cout << "Not removed.\n";
        return;
    }

    string number = accountList[id].getAccountNumber();

    // Remove account folder (its index will be inaccessible)
    std::filesystem::remove_all(accountList[id].getAccountFolder());

    // Remove account from list
    std::swap(accountList[id], accountList.back());
    accountList.pop_back();

    // Rebuild account numbers
    saveAccounts();

    cout << "Account " << number << " removed.\n";
}

/**
 * @brief Retrieve the bank accounts using the stored account numbers
 * 
 */
void BankSystem::retrieveAccounts() {
    std::ifstream accountFile;
    accountFile.open("./data/accountNumbers.txt");

    accountList.clear();
    string accountNumber;
    while (getline(accountFile >> std::ws, accountNumber)) {
        BankAccount acc(accountNumber);
        accountList.push_back(std::move(acc)); // Move BankAccount to a global scope
    }

    accountFile.close();
}

/**
 * @brief Retrieve a bank account using authentication
 * 
 * @param number The number of the bank account
 * @return BankAccount* The bank account retrieved
 */
BankAccount *BankSystem::authenticate(string number) {
    cout << "Enter the bank account's password:\n";
    string pass;
    cout << ">";
    cin >> pass;
    while (!cin) {
        clearCin();
        cout << ">";
        cin >> pass;
    }

    BankAccount *acc = getAccount(number);
    if (acc != nullptr && acc -> passwordMatches(pass)) return acc;
    else return nullptr;
}

/**
 * @brief Transfer a given amount of money from a bank account to another
 * 
 * @param fromAcc The bank account to withdraw from
 * @param to The account number of the bank account that receives the money
 * @param amount The amount of money transferred
 * @return true Transfer successful
 * @return false Error in transfer (invalid account or insufficient amount)
 */
bool BankSystem::transferMoney(BankAccount *fromAcc, string to, double amount) {
    BankAccount *toAcc;
    toAcc = getAccount(to);

    if (fromAcc == nullptr || toAcc == nullptr) return false;
    if (fromAcc -> getAccountBalance() < amount || amount < 0) return false;
    
    fromAcc -> withdraw(amount, false);
    fromAcc -> saveTransaction("Transferred out " + std::to_string(amount) + " to account " + to);
    toAcc -> deposit(amount, false);
    toAcc -> saveTransaction("Transferred in " + std::to_string(amount) + " from account " + fromAcc -> getAccountNumber());

    fromAcc -> saveAccountBalance();
    toAcc -> saveAccountBalance();

    return true;
}

/**
 * @brief List the account number of the current stored bank accounts
 * 
 */
void BankSystem::listAccountNumbers() {
    cout << "Account numbers stored:\n";
    for (int i = 0; i < accountList.size(); i++) {
        cout << " - " << accountList[i].getAccountNumber() << '\n';
    }
}

// Private field

/**
 * @brief Get the index of the bank account in stored list
 * 
 * @param number The account number of the bank account
 * @return int The index of the bank account in account list
 */
int BankSystem::getAccountIndex(string number) {
    // Linear search
    for (int i = 0; i < accountList.size(); i++) {
        if (accountList[i].getAccountNumber() == number) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Get the bank account using account number
 * 
 * @param number The account number of the bank account
 * @return BankAccount* The pointer to the bank account with the given account number
 */
BankAccount *BankSystem::getAccount(string number) {
    int id = getAccountIndex(number);
    if (id >= 0) {
        return &accountList[id];
    }
    return nullptr;
}

void BankSystem::saveAccounts() {
    std::ofstream accountFile;
    accountFile.open("./data/accountNumbers.txt");

    for (int i = 0; i < accountList.size(); i++) {
        accountFile << accountList[i].getAccountNumber() << '\n';
    }

    accountFile.close();
}

/**
 * @brief Save the account number to the end of a file
 * 
 * @param number The account number to be saved
 */
void BankSystem::saveAccountNumber(string number) {
    std::ofstream accountFile;
    accountFile.open("./data/accountNumbers.txt", std::ofstream::app);
    
    accountFile << number << '\n';

    accountFile.close();
}
