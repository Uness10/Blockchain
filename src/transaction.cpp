#include "transaction.h"

using namespace std;

Transaction::Transaction(const string& sender, const string& recipient, 
                         double amount, long long timestamp)
    : sender(sender), recipient(recipient), amount(amount), timestamp(timestamp) {
        hash = calculateTransactionHash();
    }

string Transaction::calculateTransactionHash() const {
    stringstream ss;
    ss << sender << recipient << amount << timestamp;
    return crypto::hash(ss.str());
}

void Transaction::signTransaction(const string& privateKeyStr) {
    signature = crypto::sign(hash, privateKeyStr);
}

bool Transaction::verifySignature(const string& publicKeyStr) const {
    return crypto::verify(hash, signature, publicKeyStr);
}
// Getters
string Transaction::getSender() const { return sender; }
string Transaction::getRecipient() const { return recipient; }
double Transaction::getAmount() const { return amount; }
long long Transaction::getTimestamp() const { return timestamp; }
string Transaction::getSignature() const { return signature; }
string Transaction::getHash() const { return hash; }
