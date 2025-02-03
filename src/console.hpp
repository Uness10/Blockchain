#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include "block.hpp"
#include "blockchain.hpp"  // Make sure Blockchain is included

class Console {
private:
    static Console* instance;  
    Console();  

public:
    Console(const Console&) = delete;
    Console& operator=(const Console&) = delete;

    static Console* getInstance();  

    void log(const std::string& message);
    void printBlock(const Block& b) const;
    void printBlockchain(const Blockchain& bc) const;
};

#endif // CONSOLE_HPP
