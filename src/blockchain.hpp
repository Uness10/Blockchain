#ifndef BLOCKCHAIN_HPP
#define BLOCKCHAIN_HPP

#include "block.hpp"
#include "console.hpp"
#include <vector>
#include <string>

class Blockchain {
private:
    std::vector<Block> chain;
    int difficulty;

public:
    Blockchain(int diff, bool init = true);

    const std::vector<Block>& getChain() const;
    void addBlock(const std::string& data);
    bool isChainValid() const;

    void exportToJSON(const std::string& fname);
    void importFromJSON(const std::string& fname);
    void exportToCSV(const std::string& fname);
    void importFromCSV(const std::string& fname);
};

#endif // BLOCKCHAIN_HPP
