#include <iostream>
#include "blockchain.h"
#include <openssl/applink.c>

using namespace std;

int main() {
    cout << "Starting Blockchain Tests...\n" << endl;

    // 🟢 **Creating Users**
    User alice("Alice");
    User bob("Bob");
    User charlie("Charlie");
    User david("David");

    // 🟢 **Initializing Blockchain**
    Blockchain myBlockchain(3);

    // 🟢 **Creating Transactions**
    Transaction tx1(alice, bob, 50.0, "2025-02-06 12:00:00");
    Transaction tx2(bob, charlie, 30.0, "2025-02-06 12:05:00");
    Transaction tx3(charlie, david, 20.0, "2025-02-06 12:10:00");
    Transaction tx4(david, alice, 10.0, "2025-02-06 12:15:00");

    // Sign transactions
    tx1.signTransaction();
    tx2.signTransaction();
    tx3.signTransaction();
    tx4.signTransaction();

    // 🟢 **Verifying Transactions**
    if (!tx1.verifySignature() || !tx2.verifySignature() || !tx3.verifySignature() || !tx4.verifySignature()) {
        cerr << ":(Transaction signature verification failed!" << endl;
        return 1;
    }
    cout << ":) All transactions verified successfully!\n" << endl;

    // 🟢 **Adding Blocks**
    myBlockchain.addBlock("Block 1");
    int block1 = myBlockchain.getChain().size() - 1;
    myBlockchain.addTransactionsToBlock(block1, tx1);
    myBlockchain.addTransactionsToBlock(block1, tx2);

    myBlockchain.addBlock("Block 2");
    int block2 = myBlockchain.getChain().size() - 1;
    myBlockchain.addTransactionsToBlock(block2, tx3);
    myBlockchain.addTransactionsToBlock(block2, tx4);

    // 🟢 **Printing Blockchain**
    myBlockchain.printChain();

    // 🟢 **Validating Blockchain**
    if (myBlockchain.isChainValid()) {
        cout << ":) Blockchain is valid!" << endl;
    } else {
        cout << ":(Blockchain validation failed!" << endl;
        return 1;
    }



    cout << "Sbe3 ! " << endl;
    return 0;
}
