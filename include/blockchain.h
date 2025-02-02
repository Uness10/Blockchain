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
    Blockchain(int diff, bool init = true);
    ~Blockchain();

    void addBlock(const std::string& data);
    bool isChainValid() const;
    void printChain() const;

    void exportToJSON(const std::string& fname);
    void importFromJSON(const std::string& fname);

    void exportToCSV(const std::string& fname);
    void importFromCSV(const std::string& fname);
};

#endif
