#include <iostream>
#include "blockchain.h"
#include "crypto.h"  // Include the crypto library
#include <openssl/applink.c>

using namespace std;

int main() {

    // Create users with dynamically loaded keys
    User alice("Alice");
    User bob("Bob");

    // Initialize blockchain
    Blockchain myBlockchain(2);

    // Create a transaction
    Transaction tx1(alice, bob, 50.0, "2025-02-06 12:00:00");

    tx1.signTransaction(); // Alice signs the transaction

    // Verify transaction signature
    if (tx1.verifySignature()) {
        cout << "Transaction signature verified " << endl;
    } else {
        cout << "Transaction signature verification failed " << endl;
        return 1;
    }

    // Add a new block with transaction
    myBlockchain.addBlock("First transaction block");
    int blockIndex = myBlockchain.getChain().size() - 1;
    myBlockchain.addTransactionsToBlock(blockIndex, tx1);

    // Print blockchain
    myBlockchain.printChain();

    // Validate blockchain
    if (myBlockchain.isChainValid()) {
        cout << "Blockchain is valid " << endl;
    } else {
        cout << "Blockchain is INVALID " << endl;
    }

    return 0;
}
