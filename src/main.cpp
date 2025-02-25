#include <iostream>
#include <csignal>
#include <fstream>
#include "blockchain.h"
#include <openssl/applink.c>

using namespace std;

void handleSegfault(int signal) {
    cerr << "Segmentation fault (signal " << signal << ") caught!" << endl;
    exit(EXIT_FAILURE); 
}

int main() {
    signal(SIGSEGV, handleSegfault);

    Blockchain bch(2, false);

    cout << "Checking file accessibility..." << endl;
    ifstream file("data/test.json");
    if (!file.is_open()) {
        cerr << "Failed to open the file: data/test.json" << endl;
        return 1;
    }
    file.close();

    try {
        cout << "Importing blockchain from JSON..." << endl;
        bch.importFromJSON("data/test.json");
        cout << "Import successful!" << endl;
    } catch (const exception& e) {
        cerr << "Exception caught during import: " << e.what() << endl;
        return 1;
    }

    if (bch.isChainValid()) {
        cout << ":) Blockchain is valid!" << endl;
    } else {
        cout << ":( Blockchain validation failed!" << endl;
        return 1;
    }

    cout << "Sbe3 ! " << endl;
    return 0;
}
