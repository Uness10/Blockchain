#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <string>
#include <openssl/sha.h>
#include "exceptions.hpp"
#include "console.hpp"

class Block {
private:
    int index;
    std::string timestamp;
    std::string data;
    std::string previousHash;
    std::string hash;
    long long nonce;

    std::string calculateHash() const;

public:
    Block(int idx, const std::string& dataInput, const std::string& prevHash);
    Block(int idx, const std::string& timestamp, const std::string& data,
          const std::string& previousHash, const std::string& hash, long long nonce);

    static Block adapt(int idx, const std::string& timestamp, const std::string& data,
                       const std::string& previousHash, const std::string& hash, long long nonce);

    void mineBlock(int difficulty);
    bool validateBlock() const;

    int getIndex() const;
    std::string getTimestamp() const;
    std::string getData() const;
    std::string getPreviousHash() const;
    std::string getHash() const;
    long long getNonce() const;
};

#endif // BLOCK_HPP
