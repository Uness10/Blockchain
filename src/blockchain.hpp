#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <vector>
#include <string>
#include "block.hpp"
#include "exceptions.hpp"
#include "json.hpp"

class Blockchain {
private:
    std::vector<Block> chain;
    int difficulty;

public:
    Blockchain(int diff, bool init = true);
    
    void addBlock(const std::string& data);
    bool isChainValid() const;
    void printChain() const;

    void exportToJSON(const std::string& fname);
    void importFromJSON(const std::string& fname);

    void exportToCSV(const std::string& fname);
    void importFromCSV(const std::string& fname);
};

#endif
