#pragma once
#include <exception>
#include <string>

using namespace std;

namespace BCE { // stands for BlockchainExceptions

    class BlockchainException : public exception {
    public:
        explicit BlockchainException(const string& message) : msg(message) {}
        const char* what() const noexcept override { return msg.c_str(); }
    private:
        string msg;
    };

    class InvalidBlockException : public BlockchainException {
    public:
        explicit InvalidBlockException(const string& message) : BlockchainException("Invalid Block: " + message) {}
    };

    class MiningException : public BlockchainException {
    public:
        explicit MiningException(const string& message) : BlockchainException("Mining Error: " + message) {}
    };

    class InvalidDataException : public  BlockchainException {
    public :
        explicit InvalidDataException(const string& message): BlockchainException("Data Error: " + message)  {}
    };

    class InvalidFormatException : public  BlockchainException {
    public :
        explicit InvalidFormatException(const string& message): BlockchainException("Invalid Format Error: " + message)  {}
    };

    class FileException : public BlockchainException {
    public:
        explicit FileException(const string& message) : BlockchainException("File Error: " + message) {}
    };

    class InvalidBlockchainException : public BlockchainException {
    public:
        explicit InvalidBlockchainException(const string& message) : BlockchainException("Invalid Blockchain: " + message) {}
    };
}
