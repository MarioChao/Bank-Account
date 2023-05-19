#ifndef __BANKACCOUNT__
#define __BANKACCOUNT__

#include <iostream>
#include <vector>
#include <fstream> // ifstream, ofstream
#include <ctime> // time
#include <cstdlib> // srand, rand
#include <filesystem> // createdirectory, exists
#include <functional> // hash
using std::vector;
using std::string;
using std::cout;

using ll = long long;
using ld = long double;

class Transaction;

class BankAccount {
    public:
        BankAccount(string);
        BankAccount(int, ld);
        ~BankAccount();
        BankAccount(const BankAccount&);
        // BankAccount &operator=(const BankAccount&); // Copy assignment (not defined)
        BankAccount(BankAccount&&) noexcept;
        BankAccount &operator=(BankAccount&&) noexcept;

        bool deposit(ld, bool = true);
        bool withdraw(ld, bool = true);
        bool transferTo(string, ld);
        
        void showInfo();
        void showAccountNumber();
        void showTransactions();

        void setPassword();
        bool passwordMatches(string);
        
        string getAccountNumber();
        string getAccountFolder();
        ld getAccountBalance();
        void saveAccountBalance();
        void retrieveAccountBalance();

        void setTransactionHistory(Transaction);
        void saveTransaction(string);
        void retrieveTransaction();
    private:
        static vector<ll> indexes;
        static bool setSeed;

        static void retrieveIndexList();
        static void saveIndexList();

        string number;
        void generateAccountNumber();
        string accountFolder;
        
        string password;
        void savePassword();
        void retrievePassword();
        string encodePassword(string);
        
        int type;
        ld balance;

        Transaction *transHistory = nullptr;
};

#endif