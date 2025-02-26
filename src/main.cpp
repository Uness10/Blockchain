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
        // Create users and generate keys
        User alice("Alice");
        alice.generateKeys();
        User bob("Bob");
        bob.generateKeys();

        // Register users with Signer
        Signer& signer = Signer::getInstance();
        signer.addUser(alice);
        signer.addUser(bob);

        // Create and sign a transaction
        Transaction tx1(
            alice.getPublicKey(),
            bob.getPublicKey(),
            50.0,
            "2023-10-04T12:00:00"
        );
        tx1.signTransaction(); // Signs using Alice's private key

        // Initialize blockchain with difficulty 4
        Blockchain blockchain(4);
        
        // Add genesis block
        blockchain.addBlock("Block 1");

        // Add transaction to the latest block
        blockchain.addTransactionsToBlock(1, tx1);

        // Check validity
        cout << "Blockchain valid? " << (blockchain.isChainValid() ? "Yes" : "No") << endl;

        // Performance test: Add 100 blocks with transactions
        auto start = high_resolution_clock::now();
        for (int i = 1; i <= 100; ++i) {
            // Create transaction
            Transaction tx(
                alice.getPublicKey(),
                bob.getPublicKey(),
                i * 10.0,
                "2023-10-04T12:00:00"
            );
            tx.signTransaction();

            // Add block and transaction
            blockchain.addBlock("Block " + to_string(i));
            int blockIdx = blockchain.getChain().size() - 1;
            blockchain.addTransactionsToBlock(blockIdx, tx);
        }
        auto end = high_resolution_clock::now();

        // Output results
        cout << "Blockchain valid after additions? " 
             << (blockchain.isChainValid() ? "Yes" : "No") << endl;
        cout << "Time to add 100 blocks: "
             << duration_cast<milliseconds>(end - start).count() << "ms" << endl;

        // Export/Import tests (optional)
        blockchain.exportToJSON("data/chain.json");
        // blockchain.importFromJSON("chain.json");

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}