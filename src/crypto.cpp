#include "crypto.h"

using namespace std;

namespace crypto {

    // 🔹 Hash Function (SHA-256)
     string hash(const string& data) {
        unsigned char hashResult[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.length(), hashResult);

        stringstream ss;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            ss << hex << setw(2) << setfill('0') << static_cast<int>(hashResult[i]);
        }
        return ss.str();
    }

    // 🔹 Sign Data with RSA Private Key
     string sign(const string& data, const string& privateKeyStr) {
        BIO *bio = BIO_new_mem_buf(privateKeyStr.c_str(), -1);
        if (!bio) throw runtime_error("Failed to create BIO for private key");

        RSA *privateKey = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);
        BIO_free(bio);
        if (!privateKey) throw runtime_error("Failed to read private key for signing");

        // Compute SHA-256 hash before signing
        unsigned char hashResult[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.length(), hashResult);

        // Allocate correct buffer size for signature
        vector<unsigned char> signatureBuffer(RSA_size(privateKey));
        unsigned int signatureLength;

        int result = RSA_sign(NID_sha256,
            hashResult, SHA256_DIGEST_LENGTH,  // Use computed hash
            signatureBuffer.data(), &signatureLength, privateKey);
        
        RSA_free(privateKey);

        if (result != 1) throw runtime_error("Transaction signing failed");

        return hexEncode(vector<unsigned char>(signatureBuffer.begin(), signatureBuffer.begin() + signatureLength));
    }

    // 🔹 Verify Signature with RSA Public Key
     bool verify(const string& data, const string& signature, const string& publicKeyStr) {
        BIO *bio = BIO_new_mem_buf(publicKeyStr.c_str(), -1);
        if (!bio) throw runtime_error("Failed to create BIO for public key");

        RSA *publicKey = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL); // Correct function for X.509 keys
        BIO_free(bio);
        if (!publicKey) throw runtime_error("Failed to read public key for verification");

        // Compute SHA-256 hash before verification
        unsigned char hashResult[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.length(), hashResult);

        vector<unsigned char> signatureBytes = hexDecode(signature);

        int result = RSA_verify(NID_sha256,
            hashResult, SHA256_DIGEST_LENGTH,  // Use computed hash
            signatureBytes.data(), signatureBytes.size(), publicKey);

        RSA_free(publicKey);
        return result == 1;
    }

    // 🔹 Convert Binary Data to Hex String
     string hexEncode(const vector<unsigned char>& data) {
        stringstream ss;
        for (unsigned char byte : data) {
            ss << setw(2) << setfill('0') << hex << static_cast<int>(byte);
        }
        return ss.str();
    }

    // 🔹 Convert Hex String Back to Binary Data
     vector<unsigned char> hexDecode(const string& hex) {
        vector<unsigned char> bytes(hex.length() / 2);
        for (size_t i = 0; i < bytes.size(); i++) {
            unsigned int byteValue;
            sscanf(hex.substr(i * 2, 2).c_str(), "%02x", &byteValue);
            bytes[i] = static_cast<unsigned char>(byteValue);
        }
        return bytes;
    }

}
