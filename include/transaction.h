#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <iomanip>

#include "crypto.h"

using namespace std;
class Transaction {
    string sender;     // Sender's public key
    string recipient;  // Recipient's public key
    double amount;          // Transaction amount
    long long timestamp;    // Transaction timestamp
    string hash ;
    string signature; 
public:
    // Constructor for creating a new transaction
    Transaction(const string& sender, const string& recipient, 
                double amount, long long timestamp);

    // Verify transaction signature
    bool verifySignature(const string& publicKeyStr) const;

    // Sign transaction with private key
    void signTransaction(const string& privateKeyStr);

    // Getters
    string getSender() const;
    string getRecipient() const;
    double getAmount() const;
    long long getTimestamp() const;
    string getSignature() const;
    string getHash() const ;
    // Generate a hash of the transaction data
    string calculateTransactionHash() const;


};

#endif