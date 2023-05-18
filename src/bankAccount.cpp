#include "../inc/bankAccount.h"
#include "../inc/transaction.h"
#include "../inc/bankSystem.h"
#include "../inc/cinFunctions.h"

// Static declaration

vector<ll> BankAccount::indexes;
bool BankAccount::setSeed = 0;

// Public field

/**
 * @brief Construct a new Bank Account:: Bank Account object representing an existing account
 * 
 * @param number The bank number
 */
BankAccount::BankAccount(string number) {
    // Check whether account number exist
    this -> number = number;
    accountFolder = "./data/accounts/BANK" + number;
    if (!std::filesystem::exists(accountFolder)) {
        throw "No bank account found";
    }

    // Get password
    retrievePassword();

    // Get account type
    int firstHyph = number.find('-');
    int secondHyph = number.find('-', firstHyph + 1);
    type = stoi(number.substr(firstHyph + 1, secondHyph - firstHyph - 1));
    // Get account balance
    retrieveAccountBalance();

    // Get transaction history
    retrieveTransaction();
}

/**
 * @brief Construct a new Bank Account:: Bank Account object
 * 
 * @param type The type of account
 * @param bal The amount of balance in the account
 */
BankAccount::BankAccount(int type, double bal) {
    if (type > 999) type = 999;
    if (type < 0) type = 0;

    // Set type
    this -> type = type;
    // Set balance
    this -> balance = bal;

    // Set account number
    generateAccountNumber();
    // Set password
    setPassword();

    // Create account folder
    accountFolder = "./data/accounts/BANK" + number;
    std::filesystem::create_directory(accountFolder);

    // Save account balance
    saveAccountBalance();
    // Save password
    savePassword();

    // Set and save transaction history
    transHistory = nullptr;
    saveTransaction("Account created with $" + std::to_string(balance));

    // Add account to the bank system
    BankSystem::addAccountToList(this);
}

/**
 * @brief Destroy the Bank Account:: Bank Account object
 * 
 */
BankAccount::~BankAccount() {
    delete transHistory;
}

/**
 * @brief Copy constructor for a Bank Account object
 * 
 * @param o The object copy from
 */
BankAccount::BankAccount(const BankAccount& o) {
    number = o.number;
    accountFolder = o.accountFolder;
    password = o.password;
    type = o.type;
    balance = o.balance;
    transHistory = o.transHistory;
}

/**
 * @brief Move constructor for a Bank Account object
 * 
 * @param o The object to move from
 */
BankAccount::BankAccount(BankAccount&& o) noexcept {
    number = o.number;
    accountFolder = o.accountFolder;
    password = o.password;
    type = o.type;
    balance = o.balance;
    transHistory = std::move(o.transHistory);
    o.transHistory = nullptr;
}

/**
 * @brief Move assignment for a Bank Account object
 * 
 * @param o The object to assign from
 * @return BankAccount& 
 */
BankAccount &BankAccount::operator=(BankAccount&& o) noexcept {
    cout << "move assign\n" << std::flush;
    number = o.number;
    accountFolder = o.accountFolder;
    password = o.password;
    type = o.type;
    balance = o.balance;
    transHistory = std::move(o.transHistory);
    o.transHistory = nullptr;
    return *this;
}

/**
 * @brief Add the given amount to the account balance
 * 
 * @param amount The amount of money deposited
 * @return true Deposit successful
 * @return false Error in deposite (invalid amount)
 */
bool BankAccount::deposit(double amount, bool saveTrans) {
    if (amount < 0) return false;

    balance += amount;
    if (saveTrans) saveTransaction("Deposited " + std::to_string(amount));
    saveAccountBalance();
    
    return true;
}

/**
 * @brief Take away the given amount of money from the account balance
 * 
 * @param amount The amount of money withdrawn
 * @return true Withdraw successful
 * @return false Error in withdrawal (invalid amount)
 */
bool BankAccount::withdraw(double amount, bool saveTrans) {
    if (balance < amount || amount < 0) return false;

    balance -= amount;
    if (saveTrans) saveTransaction("Withdrawn " + std::to_string(amount));
    saveAccountBalance();

    return true;
}

/**
 * @brief Transfer a given amount of money to a second account
 * 
 * @param to The number of the account that will receive the transfer
 * @param amount How many money will be transferred
 * @return true Transfer successful
 * @return false Error in transfer (invalid amount or number)
 */
bool BankAccount::transferTo(string to, double amount) {
    return BankSystem::transferMoney(this, to, amount);
}

/**
 * @brief Display the account balance
 * 
 */
void BankAccount::showInfo() {
    cout << "Your account balance: $" << balance << '\n';
}

/**
 * @brief Display the account number
 * 
 */
void BankAccount::showAccountNumber() {
    cout << "Account number: " << number << '\n';
}

/**
 * @brief Show the transaction history of this bank account
 * 
 */
void BankAccount::showTransactions() {
    if (transHistory == nullptr) return;

    auto history = transHistory -> getHistory();

    cout << "o - - - - - - - - - - - - - - - - - - - - o\n";
    showAccountNumber();
    cout << '\n';
    for (int i = 0; i < history.size(); i++) {
        cout << history[i].first << '\n';
        cout << '\t' << history[i].second << "\n\n";
    }
    cout << "o - - - - - - - - - - - - - - - - - - - - o\n";
}

/**
 * @brief Set the password of the account
 * 
 */
void BankAccount::setPassword() {
    cout << "Enter password:\n";
    cout << "(For safety reasons, please use a non-personal password)\n";
    string pass;
    cout << ">";
    cin >> pass;
    while (!cin) {
        clearCin();
        cout << ">";
        cin >> pass;
    }

    password = encodePassword(pass);
    cout << "Password set!\n";
}

/**
 * @brief Return whether a given string matches the password
 * 
 * @param enter The input string
 * @return true Matches
 * @return false Don't match
 */
bool BankAccount::passwordMatches(string enter) {
    return encodePassword(enter) == password;
}

/**
 * @brief Get the account number of the bank account
 * 
 * @return string The account number of the bank account
 */
string BankAccount::getAccountNumber() {
    return number;
}

/**
 * @brief Get the account folder of the bank account
 * 
 * @return string The path to the account folder
 */
string BankAccount::getAccountFolder() {
    return accountFolder;
}

/**
 * @brief Get the account balance of the bank account
 * 
 * @return double The account balance of the bank account
 */
double BankAccount::getAccountBalance() {
    return balance;
}

/**
 * @brief Save the account balance in a file
 * 
 */
void BankAccount::saveAccountBalance() {
    std::ofstream balFile;
    balFile.open(accountFolder + "/verySecureBalance.txt");

    balFile << balance;

    balFile.close();
}

/**
 * @brief Retreive the account balance from a file
 * 
 */
void BankAccount::retrieveAccountBalance() {
    std::ifstream balFile;
    balFile.open(accountFolder + "/verySecureBalance.txt");

    string balStr;
    getline(balFile >> std::ws, balStr);
    balance = stod(balStr);

    balFile.close();
}

/**
 * @brief Set the transaction history of this bank account to a given Transaction object
 * 
 * @param trans The account's upcoming transaction history
 */
void BankAccount::setTransactionHistory(Transaction trans) {
    transHistory = &trans;
}

/**
 * @brief Save the transaction to transaction history
 * 
 * @param trans A string representing the transaction
 */
void BankAccount::saveTransaction(string trans) {
    if (transHistory == nullptr) transHistory = new Transaction();
    transHistory -> storeTransaction(trans);
    
    std::ofstream transFile;
    transFile.open(accountFolder + "/transHistory.txt", std::ofstream::app);

    auto transPair = transHistory -> getLastTransaction();
    transFile << transPair.first << transPair.second << "\n";

    transFile.close();
}

/**
 * @brief Retrieve the saved transaction
 * 
 */
void BankAccount::retrieveTransaction() {
    delete transHistory;
    transHistory = new Transaction();

    std::ifstream transFile;
    transFile.open(accountFolder + "/transHistory.txt");

    string date, info;
    while (getline(transFile >> std::ws, date)) {
        getline(transFile >> std::ws, info);
        transHistory -> storeTransaction(date + '\n', info);
    }

    transFile.close();
}

// Private field

/**
 * @brief Retrieve the list of account-type indexes from a file
 * 
 */
void BankAccount::retrieveIndexList() {
    indexes.clear(), indexes.resize(1000);
    std::ifstream indexFile;
    indexFile.open("./data/accountIndexes.txt");
    
    int lineId = 0;
    string line;
    while (getline(indexFile >> std::ws, line) && (lineId < 1000)) {
        if (line == "") indexes[lineId] = 0;
        else indexes[lineId] = stoll(line);
        lineId++;
    }

    indexFile.close();
}

/**
 * @brief Save the list of account-type indexes to a file
 * 
 */
void BankAccount::saveIndexList() {
    std::ofstream indexFile;
    indexFile.open("./data/accountIndexes.txt");
    
    for (int i = 0; i < indexes.size(); i++) {
        indexFile << indexes[i] << '\n';
    }

    indexFile.close();
}

/**
 * @brief Generate and set the account number based on branch, type, index, and rng
 * 
 */
void BankAccount::generateAccountNumber() {
    // Really just a random number consistent across bank accounts
    string branchStr = "123";
    
    // The number based on the type of account
    string typeStr = std::to_string(type);

    // Unique index based on type of account
    if (indexes.empty()) retrieveIndexList();
    string indexStr = std::to_string(indexes[type]++);
    indexStr = string(10 - indexStr.length(), '0') + indexStr; // Pad index to 10 digits
    saveIndexList();
    
    // Set random seed if not
    if (!setSeed) {
        srand(time(0));
        setSeed = 1;
    }
    // Random 3-digit number
    string rngStr = std::to_string(rand() % 1000);
    rngStr = string(3 - rngStr.length(), '0') + rngStr;

    // A combination of the four numbers separated by hyphens
    number = branchStr + "-" + typeStr + "-" + indexStr + "-" + rngStr;
}

/**
 * @brief Save the account password
 * 
 */
void BankAccount::savePassword() {
    std::ofstream passFile;
    passFile.open(accountFolder + "/passFlag.txt");

    passFile << password;

    passFile.close();
}

/**
 * @brief Retrieve the account password
 * 
 */
void BankAccount::retrievePassword() {
    std::ifstream passFile;
    passFile.open(accountFolder + "/passFlag.txt");

    string pass;
    getline(passFile >> std::ws, pass);
    password = pass;

    passFile.close();
}

/**
 * @brief Return an encoded string representing the password
 * 
 * @param pass The initial string
 * @return string The encoded string
 */
string BankAccount::encodePassword(string pass) {
    return std::to_string(std::hash<string>()(pass));
}