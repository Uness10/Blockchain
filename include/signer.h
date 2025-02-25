#ifndef SIGNER_H
#define SIGNER_H

#include <map>
#include <string>
#include <stdexcept>
#include "user.h"
#include "crypto.h"

using namespace std;

class Signer {
private:
    map<string, User> keyStore;

    Signer() = default;

    Signer(const Signer&) = delete;
    Signer& operator=(const Signer&) = delete;

public:
    static Signer& getInstance() {
        static Signer instance; 
        return instance;
    }

    void addUser(const User& u) {
        keyStore[u.getPublicKey()] = u;
    }

    bool exists(const string& pk) const {
        return keyStore.find(pk) != keyStore.end();
    }

    string signTransaction(const string& senderPK, const string& txHash) {
        if (!exists(senderPK))
            throw runtime_error("Private key not found for the given public key!");

        string sk = keyStore[senderPK].getPrivateKey();
        string signature = crypto::sign(txHash, sk);
        return signature;
    }
};

#endif
