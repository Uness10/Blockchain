#include "blockchain.h"


using namespace std;
using json = nlohmann::json;
using namespace BCE;

int calculateDifficulty(const vector<Block>& chain ){
    int diff = INT_MAX ; 
    for(const Block& b: chain){
        int i = 0 ; 
        string bHash = b.getHash();
        while(bHash[i]=='0' && i<bHash.size())
            i++; 
        diff = min(diff,i);
    }
    return diff; 
}

Blockchain::Blockchain(int diff) : difficulty(diff) {
    logger = new Logger();
        Block genesisBlock(0, "Genesis Block", "0");
        genesisBlock.mineBlock(difficulty);
        chain.push_back(genesisBlock);
        logger->log("Genesis block created.");
}
Blockchain::Blockchain() {
    logger = new Logger();

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
    for (size_t i = 1; i < chain.size()-1; i++) {
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

    difficulty = calculateDifficulty(tempChain);

    chain = move(tempChain);
    if (!isChainValid()) {
        throw InvalidBlockchainException("Imported blockchain is invalid!");
    }
    if (logger) logger->log("Blockchain successfully imported from " + fname);
}




Block Blockchain::getBlock(int idx)  const  { return chain[idx]; }
vector<Block> Blockchain::getChain() const { return chain;} 
