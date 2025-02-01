cmake_minimum_required(VERSION 3.14)

# Set project details
project(BlockchainProject VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Print OpenSSL include directory for debugging
message(STATUS "OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")

# Add executable and source files
add_executable(blockchain src/blockchain.cpp)

# Include directories for external libraries
target_include_directories(blockchain PRIVATE
    ${OPENSSL_INCLUDE_DIR}               # OpenSSL headers
    ${CMAKE_CURRENT_SOURCE_DIR}/include  # Include manually downloaded json.hpp
)

# Link libraries
target_link_libraries(blockchain PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
)
