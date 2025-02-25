#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <iomanip>

#include "crypto.h"
#include "signer.h"
using namespace std;

class Transaction {
private:
    const string spk;      // Sender's public key 
    const string rpk;   // Recipient's public key
    double amount;           // Transaction amount
    string timestamp;        // Transaction timestamp
    string hash;
    string signature; 

    // Private constructor for internal use
    Transaction(const string& sender, const string& recipient, 
                double amount, const string& timestamp, const string& signature);

public:
    // Constructor for creating a new transaction
    Transaction(const string& sender, const string& recipient, 
                double amount, const string& timestamp);

    // Adapt an existing transaction from JSON data
    static Transaction adapt(const string& sender, const string& recipient, 
                             double amount, const string& timestamp, 
                             const string& signature);

    // Verify transaction signature
    bool verifySignature() const;

    // Sign transaction with sender's private key
    void signTransaction();

    // Getters
    string getSenderPublicKey() const;
    string getRecipientPublicKey() const;
    double getAmount() const;
    string getTimestamp() const;
    string getSignature() const;
    string getHash() const;

    // Generate a hash of the transaction data
    string calculateTransactionHash() const;
};

#endif
