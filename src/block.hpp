#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <openssl/sha.h>
#include <sstream>

class Block {
private:
    int index;
    std::string timestamp;
    std::string data;
    std::string previousHash;
    std::string hash;
    long long nonce;

    std::string calculateHash() const;
   Block(int idx, const std::string& timestamp, const std::string& data, const std::string& previousHash, const std::string& hash, long long nonce);
        
public:
    Block(int idx, const std::string& dataInput, const std::string& prevHash);

    static Block adapt(int idx, const std::string& timestamp, const std::string& data, 
                       const std::string& previousHash, const std::string& hash, long long nonce);

    void mineBlock(int difficulty);
    void printBlock() const;

    int getIndex() const;
    std::string getTimestamp() const;
    std::string getData() const;
    std::string getPreviousHash() const;
    std::string getHash() const;
    long long getNonce() const;
    bool validateBlock() const;
};

#endif
