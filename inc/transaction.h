#ifndef __TRANSACTION__
#define __TRANSACTION__

#include <iostream>
#include <vector>
#include <ctime>
using std::string;
using std::vector;
using std::pair;

class Transaction {
    public:
        Transaction();
        void storeTransaction(string, string);
        void storeTransaction(string);
        vector<pair<string, string> > getHistory();
        pair<string, string> getLastTransaction();
    private:
        vector<pair<string, string> > transHistory;
};

#endif