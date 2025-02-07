#include "block.h"

using namespace std;
using namespace BCE;

Block::Block(int idx, const string& dataInput, const string& prevHash)
    : index(idx), data(dataInput), previousHash(prevHash), nonce(0) {
    time_t now = time(0);
    timestamp = string(ctime(&now));
    timestamp.pop_back();  
    hash = calculateHash();
}

Block::Block(int idx, const string& timestamp, const string& data,
             const string& previousHash, const string& hash, 
             const vector<Transaction>& txs, long long nonce)
    : index(idx), timestamp(timestamp), data(data), 
      previousHash(previousHash), hash(hash), nonce(nonce), transactions(txs) {}

Block Block::adapt(int idx, const string& timestamp, const string& data, 
                   const string& previousHash, const string& hash, 
                   const vector<Transaction>& txs, long long nonce) {
    return Block(idx, timestamp, data, previousHash, hash, txs, nonce);
}

string Block::calculateHash() const {
    stringstream ss;
    ss << index << timestamp << data << previousHash << nonce;

    for (const auto& tx : transactions) {
        ss <<  tx.getSignature();
    }

    return crypto::hash(ss.str());
}

void Block::mineBlock(int difficulty) {
    string target(difficulty, '0');
    long long attempts = 0;
    long long maxAttempts = 1000000;

    while (hash.substr(0, difficulty) != target) {
        if (++attempts > maxAttempts) {
            throw MiningException("Exceeded max attempts for block " + to_string(index));
        }
        nonce++;
        hash = calculateHash();
    }
}

bool Block::validateBlock() const {
    if (hash != calculateHash()) {
        return false;
    }

    for (const auto& tx : transactions) {
        if (!tx.verifySignature()) {  // ✅ Uses sender's public key internally
            return false;
        }
    }

    return true;
}

void Block::addTransaction(const Transaction& tx) {
    transactions.push_back(tx);
    hash = this->calculateHash();
}

// Getters
int Block::getIndex() const { return index; }
string Block::getTimestamp() const { return timestamp; }
string Block::getData() const { return data; }
string Block::getPreviousHash() const { return previousHash; }
string Block::getHash() const { return hash; }
long long Block::getNonce() const { return nonce; }
vector<Transaction> Block::getTransactions() const { return transactions; }
void Block::setPrevHash(string& hsh){
    previousHash = hsh;
}