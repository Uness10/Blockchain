#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <json.hpp>

#include "block.h"
#include "exceptions.h"
#include "logger.h"

class Blockchain {
private:
    std::vector<Block> chain;
    int difficulty;

    Logger* logger ;


public:
    Blockchain(int diff);
    Blockchain();
    ~Blockchain();

    void addBlock(const std::string& data);
    void addTransactionsToBlock(int idx,Transaction& tx);
    bool isChainValid() const;
    void printChain() const;

    void exportToJSON(const std::string& fname);
    void importFromJSON(const std::string& fname);


    Block getBlock(int idx)  const  ; 
    vector<Block> getChain() const ;
};

#endif
