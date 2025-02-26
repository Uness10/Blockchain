cmake_minimum_required(VERSION 3.14)

# Set project details
project(BlockchainProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Print OpenSSL include directory for debugging
message(STATUS "OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")

# Include directories
include_directories(
    ${OPENSSL_INCLUDE_DIR}               # OpenSSL headers
    ${CMAKE_CURRENT_SOURCE_DIR}/include  # Manually downloaded json.hpp headers
)

# Add executable and source files (use main.cpp as the main file)
add_executable(blockchain 
    src/main.cpp  # Updated to point to main.cpp
    src/blockchain.cpp  # Other source files, if any
    src/block.cpp
    src/crypto.cpp
    src/transaction.cpp
    src/user.cpp
    src/test.cpp
)

# Link libraries
target_link_libraries(blockchain PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Optionally, if your project requires installation, you can include install rules
# install(TARGETS blockchain DESTINATION bin)
