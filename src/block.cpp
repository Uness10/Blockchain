#include "block.h"


using namespace std;
using namespace BCE;

Block::Block(int idx, const string& dataInput, const string& prevHash)
    : index(idx), data(dataInput), previousHash(prevHash), nonce(0) {
    time_t now = time(0);
    timestamp = string(ctime(&now));
    timestamp.pop_back(); // Remove newline character
    hash = calculateHash();
}

Block::Block(int idx, const string& timestamp, const string& data,
             const string& previousHash, const string& hash, long long nonce)
    : index(idx), timestamp(timestamp), data(data), previousHash(previousHash), hash(hash), nonce(nonce) {}

Block Block::adapt(int idx, const string& timestamp, const string& data, 
                   const string& previousHash, const string& hash, long long nonce) {
    return Block(idx, timestamp, data, previousHash, hash, nonce);
}

string Block::calculateHash() const {
    stringstream ss;
    ss << index << timestamp << data << previousHash << nonce;
    string toHash = ss.str();

    unsigned char hashResult[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(toHash.c_str()), toHash.size(), hashResult);
    
    stringstream hashString;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        hashString << hex << setw(2) << setfill('0') << static_cast<int>(hashResult[i]);
    }
    return hashString.str();
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
    return hash == calculateHash();
}

int Block::getIndex() const { return index; }
string Block::getTimestamp() const { return timestamp; }
string Block::getData() const { return data; }
string Block::getPreviousHash() const { return previousHash; }
string Block::getHash() const { return hash; }
long long Block::getNonce() const { return nonce; }
