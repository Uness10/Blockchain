#include "console.hpp"

Console* Console::instance = nullptr;  

Console::Console() {}

Console* Console::getInstance() {  
    if (instance == nullptr) {
        instance = new Console();
    }
    return instance;
}

void Console::log(const std::string& message) {
    auto now = std::chrono::system_clock::now(); 
    time_t now_c = std::chrono::system_clock::to_time_t(now);  
    tm* now_tm = localtime(&now_c); 
    std::cout << "[" << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S") << "] " << message << std::endl;
}

void Console::printBlock(const Block& b) const {
    std::cout << "Block Index: " << b.getIndex() << std::endl;
    std::cout << "Timestamp: " << b.getTimestamp() << std::endl;
    std::cout << "Nonce: " << b.getNonce() << std::endl;
    std::cout << "Data: " << b.getData() << std::endl;
    std::cout << "Previous Hash: " << b.getPreviousHash() << std::endl;
    std::cout << "Hash: " << b.getHash() << std::endl;
}

void Console::printBlockchain(const Blockchain& bc) const {
    for (const Block& block : bc.chain) {  
        this->printBlock(block);
        std::cout << "-------------------------" << std::endl;
    }
}
