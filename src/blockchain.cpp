#include "blockchain.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <json.hpp>

using namespace std;
using namespace BCE;

using json = nlohmann::json;

Blockchain::Blockchain(int diff, bool init) : difficulty(diff) {
    if (init) {
        Block genesisBlock(0, "Genesis Block", "0");
        genesisBlock.mineBlock(difficulty);
        chain.push_back(genesisBlock);
    }
}

void Blockchain::addBlock(const string& data) {
    if (data.empty()) {
        throw InvalidDataException("Block Data cannot be empty");
    }
    try {
        Block newBlock(chain.size(), data, chain.back().getHash());
        newBlock.mineBlock(difficulty);
        chain.push_back(newBlock);
    } catch (const MiningException& e) {
        cerr << e.what() << endl;
    }
}

bool Blockchain::isChainValid() const {
    try {
        for (size_t i = 1; i < chain.size(); i++) {
            const Block& currentBlock = chain[i];
            const Block& previousBlock = chain[i - 1];

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

void Blockchain::printChain() const {
    for (const Block& block : chain) {
        block.printBlock();
        cout << "-------------------------" << endl;
    }
}

void Blockchain::exportToJSON(const string& fname) {
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
        file << data.dump(4);  // Pretty print with indentation of 4 spaces
        file.close();
        cout << "Blockchain exported to " << fname << endl;
    } else {
        throw FileException("Failed to open file: " + fname);
    }
}

void Blockchain::importFromJSON(const string& fname) {
    ifstream file(fname);
    if (!file.is_open()) {
        throw FileException("Failed to open file: " + fname);
    }
    
    json data;
    try {
        file >> data;
        file.close();
    } catch (const nlohmann::json::parse_error& e) {
        throw FileException("JSON Parsing Error - " + string(e.what()));
    }

    chain.clear();
    for (const auto& blockJ : data) {
        if (!blockJ.contains("index"))
            throw InvalidFormatException("In JSON Data - Missing index field at some Block");
        
        if (!blockJ.contains("data") || !blockJ.contains("previousHash") || 
            !blockJ.contains("nonce") || !blockJ.contains("hash") || !blockJ.contains("timestamp")) {
            throw InvalidFormatException("In JSON Data - Missing required fields at Block " + to_string(blockJ["index"]));
        }
        
        Block b = Block::adapt(
            blockJ["index"],
            blockJ["timestamp"],
            blockJ["data"],
            blockJ["previousHash"],
            blockJ["hash"],
            blockJ["nonce"]
        );

        if (!b.validateBlock()) {
            throw InvalidBlockException("In JSON data - Hash mismatch at block " + to_string(blockJ["index"]));
        }

        chain.push_back(b);
    }
    cout << "Blockchain imported from " << fname << endl;
    if (!isChainValid())
        throw InvalidBlockchainException("Imported blockchain is invalid!");
}

void Blockchain::exportToCSV(const string& fname) {
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

void Blockchain::importFromCSV(const string& fname) {
    ifstream file(fname);
    if (!file.is_open()) {
        throw FileException("Failed to open file: " + fname);
    }

    chain.clear(); 

    string line;
    getline(file, line);  // Skip the header
    int i = 2;  // Line count starts from 2 due to header
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