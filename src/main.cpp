#include "blockchain.h"
#include "exceptions.h"

using namespace BCE ; 
using namespace std ; 
int main() {
    try {
        Blockchain myChain(2, false);  
        myChain.importFromCSV("data/blockchain.csv");
        myChain.printChain();
    } catch (const BlockchainException& e) {
        cerr << e.what() << endl;
    } catch (const exception& e) {
        cerr << "General Exception: " << e.what() << endl;
    }
    return 0;
}
