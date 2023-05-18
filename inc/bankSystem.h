#ifndef __BANKSYSTEM__
#define __BANKSYSTEM__

#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
using std::string;
using std::vector;
using std::cin;
using std::cout;

class BankAccount;

class BankSystem {
    public:
        static void addAccountToList(BankAccount *);
        static void removeAccount(BankAccount *);
        static void retrieveAccounts();

        static BankAccount *authenticate(string);
        static bool transferMoney(BankAccount *, string, double);

        static void listAccountNumbers();
    private:
        static vector<BankAccount> accountList;
        static int getAccountIndex(string);
        static BankAccount *getAccount(string);
        static void saveAccounts();
        static void saveAccountNumber(string);
};

#endif