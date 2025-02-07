#include "transaction.h"

using namespace std;

Transaction::Transaction(const User& sender, const User& recipient, 
                         double amount, const string& timestamp, 
                         const string& signature) 
    : sender(sender), recipient(recipient), amount(amount), 
      timestamp(timestamp), signature(signature) {
    hash = calculateTransactionHash(); // Ensure hash is set
}

Transaction::Transaction(const User& sender, const User& recipient, 
                         double amount, const string& timestamp)
    : sender(sender), recipient(recipient), amount(amount), 
      timestamp(timestamp) {
        cout << 12<<endl;
    hash = calculateTransactionHash(); // Calculate hash for new transactions
    cout << 21<<endl;
}

Transaction Transaction::adapt(const User& sender, const User& recipient, 
                               double amount, const string& timestamp, 
                               const string& signature) {
    return Transaction(sender, recipient, amount, timestamp, signature);
}

string Transaction::calculateTransactionHash() const {
    stringstream ss;
    ss << sender.getPublicKey() << recipient.getPublicKey() 
       << amount << timestamp;
    return crypto::hash(ss.str());
}

void Transaction::signTransaction() {
    hash = calculateTransactionHash();  
    signature = crypto::sign(hash, sender.getPrivateKey());
}

bool Transaction::verifySignature() const {
    return crypto::verify(hash, signature, sender.getPublicKey());
}

// Getters
string Transaction::getSenderPublicKey() const { return sender.getPublicKey(); }
string Transaction::getRecipientPublicKey() const { return recipient.getPublicKey(); }
double Transaction::getAmount() const { return amount; }
string Transaction::getTimestamp() const { return timestamp; }
string Transaction::getSignature() const { return signature; }
string Transaction::getHash() const { return hash; }
