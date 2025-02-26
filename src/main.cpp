#include "blockchain.h"
#include <chrono>
#include <iostream>
#include <csignal>
using namespace std;
using namespace std::chrono;

void handleSegfault(int signal) {
    cerr << "Segmentation fault (signal " << signal << ") caught!" << endl;
    exit(EXIT_FAILURE); 
}
int main() {
    signal(SIGSEGV, handleSegfault);

    try {
        Blockchain blockchain;
        blockchain.importFromJSON("data/chain.json");

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}