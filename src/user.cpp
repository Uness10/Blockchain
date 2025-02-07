#include "user.h"


using namespace std;
namespace fs = std::filesystem;

User::User(const string& name)
    : name(name) {
        generateKeys();
    }

string User::getName() const { return name; }
string User::getPrivateKey() const { return privateKey; }
string User::getPublicKey() const { return publicKey; }

void User::generateKeys() {

    // Define filenames
    string pkfile = "keys/" + name + "_public.pem"; 
    string skfile = "keys/" + name + "_private.pem"; 

    // Generate key pair
    crypto::generateKeyPair(skfile, pkfile);

    // Read public key
    ifstream userPubFile(pkfile);
    if (!userPubFile) {
        throw runtime_error("Error: Failed to open public key file for " + name);
    }
    publicKey.assign((istreambuf_iterator<char>(userPubFile)), istreambuf_iterator<char>());

    // Read private key
    ifstream userPrivFile(skfile);
    if (!userPrivFile) {
        throw runtime_error("Error: Failed to open private key file for " + name);
    }
    privateKey.assign((istreambuf_iterator<char>(userPrivFile)), istreambuf_iterator<char>());

    cout << "Keys successfully generated for " << name << endl;
}
