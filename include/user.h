#ifndef USER_H
#define USER_h
#include <string>
#include "crypto.h"
#include <fstream>
#include <iostream>
#include <filesystem>

using namespace std ;

class User {
    string name ;
    string privateKey ;
    string publicKey ; 
public : 
    User(const string& name) ; 

    string getName() const ; 
    string getPrivateKey() const ;
    string getPublicKey() const ;
    void generateKeys();
};

#endif