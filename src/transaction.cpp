#include "../inc/transaction.h"

/**
 * @brief Construct a new Transaction:: Transaction object
 * 
 */
Transaction::Transaction() {
    transHistory.clear();
}

/**
 * @brief Store the transaction time-info pair inside a vector
 * 
 * @param date A string representing the current date, with \\n at the end
 * @param trans A string representing the transaction information
 */
void Transaction::storeTransaction(string date, string trans) {
    transHistory.push_back(make_pair(date, trans));
}

/**
 * @brief Store the transaction time-info pair inside a vector
 * 
 * @param trans A string representing the transaction information
 */
void Transaction::storeTransaction(string trans) {
    string date = std::ctime(new time_t(time(0)));
    transHistory.push_back(make_pair(date, trans));
}

/**
 * @brief Return the vector containing all previous transactions
 * 
 * @return vector<pair<string, string> > The vector of transaction history, each representing time and info
 */
vector<pair<string, string> > Transaction::getHistory() {
    return transHistory;
}

/**
 * @brief Return the latest transaction
 * 
 * @return pair<string, string> The latest transaction (time and info)
 */
pair<string, string> Transaction::getLastTransaction() {
    return transHistory.back();
}