#include "transaction.h"
#include "signer.h"
#include "crypto.h"
#include <sstream>
#include <stdexcept>

using namespace std;

Transaction::Transaction(const string& sender, const string& recipient, 
                         double amount, const string& timestamp, 
                         const string& signature) 
    : spk(sender), rpk(recipient), amount(amount), 
      timestamp(timestamp), signature(signature) {
    hash = calculateTransactionHash(); 
}

Transaction::Transaction(const string& sender, const string& recipient, 
                         double amount, const string& timestamp)
    : spk(sender), rpk(recipient), amount(amount), 
      timestamp(timestamp) {
    hash = calculateTransactionHash(); 
}

Transaction Transaction::adapt(const string& sender, const string& recipient, 
                               double amount, const string& timestamp, 
                               const string& signature) {
    return Transaction(sender, recipient, amount, timestamp, signature);
}

string Transaction::calculateTransactionHash() const {
    stringstream ss;
    ss << spk << rpk << amount << timestamp;
    return crypto::hash(ss.str());
}

void Transaction::signTransaction() {
    hash = calculateTransactionHash();
    signature = Signer::getInstance().signTransaction(spk, hash);
}

bool Transaction::verifySignature() const {
    return crypto::verify(hash, signature, spk);
}

// Getters
string Transaction::getSenderPublicKey() const { return spk; }
string Transaction::getRecipientPublicKey() const { return rpk; }
double Transaction::getAmount() const { return amount; }
string Transaction::getTimestamp() const { return timestamp; }
string Transaction::getSignature() const { return signature; }
string Transaction::getHash() const { return hash; }
