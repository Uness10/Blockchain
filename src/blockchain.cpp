#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <openssl/sha.h>
#include "exceptions.hpp" 
#include "json.hpp"

using json = nlohmann::json;

using namespace std;
using namespace BCE;

class Block {
    int index;
    string timestamp;
    string data;
    string previousHash;
    string hash;
    long long nonce;

    string calculateHash() const {
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
    Block(int idx, const string& timestamp, const string& data, const string& previousHash, const string& hash, long long nonce)
        : index(idx), timestamp(timestamp), data(data), previousHash(previousHash), hash(hash), nonce(nonce) {}


public:
    Block(int idx, const string& dataInput, const string& prevHash)
        : index(idx), data(dataInput), previousHash(prevHash), nonce(0) {
        time_t now = time(0);
        timestamp = string(ctime(&now));
        timestamp.pop_back(); 
        hash = calculateHash();
    }

    static Block adapt(int idx, const string& timestamp, const string& data, const string& previousHash, const string& hash, long long nonce) {
        return Block(idx, timestamp, data, previousHash, hash, nonce);
    }

    void mineBlock(int difficulty) {
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
        cout << "Block mined: " << hash << " - After " << nonce << " attempts" << endl;
    }

    int getIndex() const { return index; }
    string getTimestamp() const { return timestamp; }
    string getData() const { return data; }
    string getPreviousHash() const { return previousHash; }
    string getHash() const { return hash; }
    long long getNonce() const { return nonce; }
    bool validateBlock() const { return hash == calculateHash(); }
    
    void printBlock() const {
        cout << "Block Index: " << index << endl;
        cout << "Timestamp: " << timestamp << endl;
        cout << "Nonce: " << nonce << endl;
        cout << "Data: " << data << endl;
        cout << "Previous Hash: " << previousHash << endl;
        cout << "Hash: " << hash << endl;
    }
};

class Blockchain {
    vector<Block> chain;
    int difficulty;
    

public:
    Blockchain(int diff, bool init = true):  difficulty(diff){
        
        if(init)  {
            Block genesisBlock(0, "Genesis Block", "0");
            genesisBlock.mineBlock(difficulty);
            chain.push_back(genesisBlock);
        } 
    
    }

    void addBlock(const string& data) {
        if(data.empty())
            throw InvalidDataException("Block Data cannot be empty") ;
        try {
            Block newBlock(chain.size(), data, chain.back().getHash());
            newBlock.mineBlock(difficulty);
            chain.push_back(newBlock);
        } catch (const MiningException& e) {
            cerr << e.what() << endl;
        }
    }

    bool isChainValid() const {
        try {
            for (size_t i = 1; i < chain.size(); i++) {
                const Block& currentBlock = chain[i];
                const Block& previousBlock = chain[i - 1];

                // if (!currentBlock.validateBlock()) {
                //     throw InvalidBlockException("Hash mismatch at block " + to_string(currentBlock.getIndex()));
                // }

                if (currentBlock.getPreviousHash() != previousBlock.getHash()) {
                    throw InvalidBlockException("Previous hash mismatch at block " + to_string(currentBlock.getIndex()));
                }
            }
        } catch (const InvalidBlockException& e) {
            cerr << e.what() << endl;
            return false;
        }
        return true;
    }

    void printChain() const {
        for (const Block& block : chain) {
            block.printBlock();
            cout << "-------------------------" << endl;
        }
    }

    void exportToJSON(const string& fname) {
        json data;
        for (const Block& b : chain) {
            json block;
            block["index"] = b.getIndex();
            block["timestamp"] = b.getTimestamp();
            block["data"] = b.getData();
            block["previousHash"] = b.getPreviousHash();
            block["hash"] = b.getHash();
            block["nonce"] = b.getNonce();
            data.push_back(block);
        }
        
        ofstream file(fname);
        if (file.is_open()) {
            file << data.dump(4);
            file.close();
            cout << "Blockchain exported to " << fname << endl;
        } else {
            throw FileException("Failed to open file: " + fname);
        }
    }

    void importFromJSON(const string& fname) {
        ifstream file(fname);
        if (!file.is_open())
            throw FileException("Failed to open file: " + fname);
        
        json data;
        try {
            file >> data;  
            file.close();
        } catch (const nlohmann::json::parse_error& e) {
            throw FileException("JSON Parsing Error - "+ string(e.what()) ) ;
        }

        chain.clear();
        for (const auto& blockJ : data) {
            if(!blockJ.contains("index"))
                    throw InvalidFormatException("In JSON Data - Missing index field at some Block");
            if (!blockJ.contains("data") || !blockJ.contains("previousHash") ||!blockJ.contains("nonce") || !blockJ.contains("hash") || !blockJ.contains("timestamp")) 
                    throw InvalidFormatException("In JSON Data - Missing required fields at Block "+ to_string(blockJ["index"]));
            
            Block b = Block::adapt(
                blockJ["index"],
                blockJ["timestamp"],
                blockJ["data"],
                blockJ["previousHash"],
                blockJ["hash"],
                blockJ["nonce"]
            );

            if (!b.validateBlock()) 
                throw InvalidBlockException("In JSON data - Hash mismatch at block " + to_string(blockJ["index"]));
            
            chain.push_back(b);
        }
        cout << "Blockchain imported from " << fname << endl;
        if (!isChainValid())
            throw InvalidBlockchainException("Imported blockchain is invalid!");
    }
    
    void exportToCSV(const string& fname) {
        ofstream file(fname);
        if (!file.is_open()) {
            throw FileException("Failed to open file: " + fname);
        }

        file << "Index,Timestamp,Data,Previous Hash,Hash,Nonce\n";

        for (const Block& b : chain) {
            file << b.getIndex() << ","
                 << "\"" << b.getTimestamp() << "\","
                 << "\"" << b.getData() << "\","
                 << b.getPreviousHash() << ","
                 << b.getHash() << ","
                 << b.getNonce() << "\n";
        }

        file.close();
        cout << "Blockchain exported to " << fname << endl;
    }

    void importFromCSV(const string& fname) {
        ifstream file(fname);
        if (!file.is_open()) {
            throw FileException("Failed to open file: " + fname);
        }

        chain.clear(); 

        string line;
        getline(file, line);
        int i = 2 ; 
        while (getline(file, line)) {
            stringstream ss(line);
            string indexStr, timestamp, data, previousHash, hash, nonceStr;

            // Read values and check for missing fields
            if (!getline(ss, indexStr, ',') || 
                !getline(ss, timestamp, ',') || 
                !getline(ss, data, ',') || 
                !getline(ss, previousHash, ',') || 
                !getline(ss, hash, ',') || 
                !getline(ss, nonceStr, ',')) {
                throw InvalidFormatException("In CSV data - Missing required fields in line: " + to_string(i));
            }

            // Remove quotes from timestamp and data
            timestamp = timestamp.substr(1, timestamp.size() - 2);
            data = data.substr(1, data.size() - 2);

            // Convert index and nonce to integers, with error handling for invalid conversion
            int index = 0;
            long long nonce = 0;
            try {
                index = stoi(indexStr);
                nonce = stoll(nonceStr);
            } catch (const invalid_argument& e) {
                throw InvalidFormatException("In CSV data - Invalid number format in line: " + to_string(i));
            } catch (const out_of_range& e) {
                throw InvalidFormatException("In CSV data - Number out of range in line: " + to_string(i));
            }

            // Create a block from the CSV data
            Block b = Block::adapt(index, timestamp, data, previousHash, hash, nonce);
            // Validate the block
            if (!b.validateBlock()) {
                throw InvalidBlockException("In CSV data - Hash mismatch at block " + indexStr);
            }

            chain.push_back(b);
            i++;
        }

        file.close();
        cout << "Blockchain imported from " << fname << endl;

        // Validate the entire chain
        if (!isChainValid()) {
            throw InvalidBlockchainException("Imported blockchain is invalid!");
        }
    }

};


int main() {
    try {
        Blockchain myChain(2);  
         myChain.addBlock("Block 1 data");
         myChain.addBlock("Block 2 data");
         myChain.addBlock("Block 3 data");
         myChain.addBlock("Block 4 data");
         myChain.addBlock("Block 5 data");

        // Export the blockchain to a JSON file
       myChain.exportToJSON("data/blockchain.json");

        // Import and print the blockchain from the JSON file
        //myChain.importFromCSV("data/blockchain.csv");
       // myChain.printChain();
    } catch (const BlockchainException& e) {
        cerr << e.what() << endl;
    } catch (const exception& e) {
        cerr << "General Exception: " << e.what() << endl;
    }
    return 0;
}
