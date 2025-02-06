#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "exceptions.h"
#include "crypto.h"
#include "transaction.h"

using namespace std;

class Block {
private:
    int index;
    string timestamp;
    string data;
    string previousHash;
    string hash;
    vector<Transaction> transactions;
    long long nonce;

    string calculateHash() const;

    // Private constructor for adaptation
    Block(int idx, const string& timestamp, const string& data, 
          const string& previousHash, const string& hash, 
          const vector<Transaction>& transactions, long long nonce);
        
public:
    // Main constructor
    Block(int idx, const string& dataInput, const string& prevHash);

    // Static method to adapt existing block data
    static Block adapt(int idx, const string& timestamp, const string& data, 
                       const string& previousHash, const string& hash, 
                       const vector<Transaction>& transactions, long long nonce);

    // Mining function
    void mineBlock(int difficulty);

    // Getters
    int getIndex() const;
    string getTimestamp() const;
    string getData() const;
    string getPreviousHash() const;
    string getHash() const;
    long long getNonce() const;
    vector<Transaction> Block::getTransactions() const;
    // Validate block
    bool validateBlock() const;
};

#endif // BLOCK_H
