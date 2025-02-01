
# Blockchain Project

This project demonstrates a simple blockchain implementation in C++. It allows you to create a blockchain, add blocks, export the blockchain to a CSV or JSON file, and import it back.

## Prerequisites
Before you begin, ensure you have the following installed:

### C++ Compiler:
- **Linux/macOS**: Install g++ or clang.
- **Windows**: Install MinGW or use Visual Studio.

### CMake (version 3.14 or higher):
Download and install CMake from [here](https://cmake.org/download/).

### Git (optional, for cloning the repository):
Download and install Git from [here](https://git-scm.com/downloads).

## Project Structure
The project is organized as follows:

```
project/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── Blockchain.cpp
│   ├── Blockchain.h
│   ├── Block.cpp
│   ├── Block.h
│   └── BlockchainException.h
├── files/
│   └── blockchain.csv
└── build/
```

## Step 1: Clone the Repository
If you’re using Git, clone the repository:

```bash
git clone https://github.com/Uness10/BlockchainInCpp.git
cd BlockchainInCpp
```

If you’re not using Git, download the project files and extract them to a directory.

## Step 2: Build the Project
Navigate to the project directory:

```bash
cd BlockchainInCpp
```

Create a build directory and navigate into it:

```bash
mkdir build
cd build
```

Generate the build files using CMake:

```bash
cmake ..
```

Build the project:

```bash
cmake --build .
```

## Step 3: Run the Project
After building the project, run the executable:


### On Windows:

```bash
cd .. 
./build/Debug/blockchain.exe
```
