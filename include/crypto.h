#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#include <sstream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <string>
#include <iostream>
using namespace std ;
namespace crypto {

    void generateKeyPair(const string& privateKeyFile, const string& publicKeyFile);

    // 🔹 Hash Function (SHA-256)
    string hash(const string& data);

    
    // 🔹 Sign Data with RSA Private Key
    string sign(const string& data, const string& privateKeyStr);

    // 🔹 Verify Signature with RSA Public Key
    bool verify(const string& data, const string& signature, const string& publicKeyStr);

    // 🔹 Convert Binary Data to Hex String
    string hexEncode(const vector<unsigned char>& data);

    // 🔹 Convert Hex String Back to Binary Data
    vector<unsigned char> hexDecode(const string& hex);

}

#endif // CRYPTO_H
