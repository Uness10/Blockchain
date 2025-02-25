#include "blockchain.h"


using namespace std;
using json = nlohmann::json;
using namespace BCE;

Blockchain::Blockchain(int diff, bool init) : difficulty(diff) {
    logger = new Logger();
    if (init) {
        Block genesisBlock(0, "Genesis Block", "0");
        genesisBlock.mineBlock(difficulty);
        chain.push_back(genesisBlock);
        logger->log("Genesis block created.");
    }
}

Blockchain::~Blockchain() {
    delete logger;  
}

void Blockchain::printChain() const {
    std::cout << "===== Blockchain Start =====" << std::endl;
    for (const auto& block : chain) {
        std::cout << "Block #" << block.getIndex() << std::endl;
        std::cout << "Timestamp: " << block.getTimestamp() << std::endl;
        std::cout << "Data: " << block.getData() << std::endl;
        std::cout << "Previous Hash: " << block.getPreviousHash() << std::endl;
        std::cout << "Hash: " << block.getHash() << std::endl;
        std::cout << "Nonce: " << block.getNonce() << std::endl;
        std::cout << "--------------------------" << std::endl;
    }
    std::cout << "===== Blockchain End =====" << std::endl;
}

void Blockchain::addBlock(const string& data) {
    if (data.empty()) {
        throw InvalidDataException("Block data cannot be empty");
    }
    Block newBlock(chain.size(), data, chain.back().getHash());
    newBlock.mineBlock(difficulty);
    chain.push_back(newBlock);
    logger->log("Block added to Blockchain. Current length : "+ to_string(newBlock.getIndex()));
    // logger->log("Block mined: " + hash + " - After " + to_string(nonce) + " attempts");
}
void Blockchain::addTransactionsToBlock(int idx, Transaction& tx) {
    if (idx < 0 || idx >= chain.size()) {
        throw InvalidDataException("Block index out of range.");
    }

    Block* b = &chain[idx];
    b->addTransaction(tx);
    b->mineBlock(difficulty);

    for (size_t i = idx + 1; i < chain.size(); i++) {
        Block* ele = &chain[i] ; 
        ele->setPrevHash(this->chain[i - 1].getHash());
        ele->mineBlock(difficulty);
    }

    logger->log("Transaction added, blockchain updated from block " + to_string(idx));
}


bool Blockchain::isChainValid() const {
    for (size_t i = 1; i < chain.size(); i++) {
        const Block& currentBlock = chain[i];
        const Block& previousBlock = chain[i - 1];

        if (currentBlock.getPreviousHash() != previousBlock.getHash()) {
            throw InvalidBlockException("Previous hash mismatch at block " + to_string(currentBlock.getIndex()));
        }

        if (!currentBlock.validateBlock()) {
            throw InvalidBlockException("Invalid block detected at index " + to_string(currentBlock.getIndex()));
        }
    }
    return true;
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

        json transactions = json::array();
        for (const Transaction& t : b.getTransactions()) {  
            transactions.push_back({
                {"sender", t.getSenderPublicKey()},
                {"recipient", t.getRecipientPublicKey()},
                {"amount", t.getAmount()},
                {"timestamp", t.getTimestamp()},
                {"signature", t.getSignature()}
            });
        }
        block["transactions"] = transactions;  
        block["nonce"] = b.getNonce();

        data.push_back(block);
    }

    ofstream file(fname);
    if (file.is_open()) {
        file << data.dump(4);
        file.close();
        if (logger) logger->log("Blockchain exported to " + fname);
    } else {
        throw FileException("Failed to open file: " + fname);
    }
}


void Blockchain::importFromJSON(const string& fname) {
    ifstream file(fname);
    if (!file) {
        throw FileException("Failed to open file: " + fname);
    }

    json data;
    try {
        file >> data;
    } catch (const nlohmann::json::parse_error& e) {
        throw FileException("JSON Parsing Error: " + string(e.what()));
    }

    vector<Block> tempChain; 

    for (const auto& blockJ : data) {
        static const vector<string> blockFields = 
            {"index", "data", "previousHash", "nonce", "hash", "timestamp", "transactions"};
        
        for (const auto& field : blockFields) {
            if (!blockJ.contains(field)) {
                throw InvalidFormatException("Missing '" + field + "' field in Block " +
                                             (blockJ.contains("index") ? to_string(blockJ["index"].get<int>()) : "Unknown"));
            }
        }

        vector<Transaction> txs;
        for (const auto& txJ : blockJ["transactions"]) {
            static const vector<string> txFields = 
                {"sender", "recipient", "amount", "timestamp", "signature"};

            for (const auto& field : txFields) {
                if (!txJ.contains(field)) {
                    throw InvalidFormatException("Transaction missing '" + field +
                                                 "' in Block " + to_string(blockJ["index"].get<int>()));
                }
            }

            txs.push_back(Transaction::adapt(
                txJ["sender"],
                txJ["recipient"],
                txJ["amount"],
                txJ["timestamp"],
                txJ["signature"]
            ));
        }

        Block b = Block::adapt(
            blockJ["index"],
            blockJ["timestamp"],
            blockJ["data"],
            blockJ["previousHash"],
            blockJ["hash"],
            txs,
            blockJ["nonce"]
        );

        if (!b.validateBlock()) {
            throw InvalidBlockException("Corrupted block detected: " + to_string(blockJ["index"]));
        }

        tempChain.push_back(b);
    }



    chain = move(tempChain);
    if (!isChainValid()) {
        throw InvalidBlockchainException("Imported blockchain is invalid!");
    }
    if (logger) logger->log("Blockchain successfully imported from " + fname);
}

Block Blockchain::getBlock(int idx)  const  { return chain[idx]; }
vector<Block> Blockchain::getChain() const { return chain;} 


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
    logger->log("Blockchain exported to " + fname);
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

        if (!getline(ss, indexStr, ',') || 
            !getline(ss, timestamp, ',') || 
            !getline(ss, data, ',') || 
            !getline(ss, previousHash, ',') || 
            !getline(ss, hash, ',') || 
            !getline(ss, nonceStr, ',')) {
            throw InvalidFormatException("In CSV data - Missing required fields in line: " + to_string(i));
        }

        timestamp = timestamp.substr(1, timestamp.size() - 2);
        data = data.substr(1, data.size() - 2);

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
        vector<Transaction> txs = {};
        Block b = Block::adapt(index, timestamp, data, previousHash, hash, txs, nonce);
        if (!b.validateBlock()) {
            throw InvalidBlockException("In CSV data - Hash mismatch at block " + indexStr);
        }

        chain.push_back(b);
        i++;
    }

    file.close();
    logger->log("Blockchain imported from " + fname);

    if (!isChainValid()) {
        throw InvalidBlockchainException("Imported blockchain is invalid!");
    }
}


