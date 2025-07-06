#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <sstream>
#include <cstring>

// Simple SHA256 implementation for genesis mining
#include <openssl/sha.h>

class uint256 {
private:
    uint32_t data[8];
    
public:
    uint256() {
        memset(data, 0, sizeof(data));
    }
    
    uint256(const std::string& hex) {
        SetHex(hex);
    }
    
    void SetHex(const std::string& hex) {
        memset(data, 0, sizeof(data));
        if (hex.length() > 64) return;
        
        for (int i = 0; i < hex.length(); i += 2) {
            int byte_pos = (hex.length() - i - 2) / 2;
            int word_pos = byte_pos / 4;
            int byte_in_word = byte_pos % 4;
            
            std::string byte_str = hex.substr(i, 2);
            uint8_t byte_val = strtol(byte_str.c_str(), nullptr, 16);
            
            data[word_pos] |= (byte_val << (byte_in_word * 8));
        }
    }
    
    std::string GetHex() const {
        std::stringstream ss;
        for (int i = 7; i >= 0; i--) {
            ss << std::hex << std::setfill('0') << std::setw(8) << data[i];
        }
        return ss.str();
    }
    
    bool operator<(const uint256& other) const {
        for (int i = 7; i >= 0; i--) {
            if (data[i] < other.data[i]) return true;
            if (data[i] > other.data[i]) return false;
        }
        return false;
    }
    
    bool operator<=(const uint256& other) const {
        return *this < other || *this == other;
    }
    
    bool operator==(const uint256& other) const {
        return memcmp(data, other.data, sizeof(data)) == 0;
    }
};

// Simple block header structure
struct BlockHeader {
    uint32_t nVersion;
    uint256 hashPrevBlock;
    uint256 hashMerkleRoot;
    uint32_t nTime;
    uint32_t nBits;
    uint32_t nNonce;
    
    std::string Serialize() const {
        std::stringstream ss;
        
        // Version (4 bytes, little endian)
        for (int i = 0; i < 4; i++) {
            ss << (char)((nVersion >> (i * 8)) & 0xFF);
        }
        
        // Previous block hash (32 bytes)
        std::string prevHash = hashPrevBlock.GetHex();
        for (int i = 62; i >= 0; i -= 2) {
            ss << (char)strtol(prevHash.substr(i, 2).c_str(), nullptr, 16);
        }
        
        // Merkle root (32 bytes)
        std::string merkleRoot = hashMerkleRoot.GetHex();
        for (int i = 62; i >= 0; i -= 2) {
            ss << (char)strtol(merkleRoot.substr(i, 2).c_str(), nullptr, 16);
        }
        
        // Time (4 bytes, little endian)
        for (int i = 0; i < 4; i++) {
            ss << (char)((nTime >> (i * 8)) & 0xFF);
        }
        
        // Bits (4 bytes, little endian)
        for (int i = 0; i < 4; i++) {
            ss << (char)((nBits >> (i * 8)) & 0xFF);
        }
        
        // Nonce (4 bytes, little endian)
        for (int i = 0; i < 4; i++) {
            ss << (char)((nNonce >> (i * 8)) & 0xFF);
        }
        
        return ss.str();
    }
};

uint256 Hash(const std::string& data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data.c_str(), data.length());
    SHA256_Final(hash, &sha256);
    
    // Double SHA256
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, hash, SHA256_DIGEST_LENGTH);
    SHA256_Final(hash, &sha256);
    
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setfill('0') << std::setw(2) << (int)hash[i];
    }
    
    return uint256(ss.str());
}

int main() {
    // CustomCoin genesis block parameters
    BlockHeader genesis;
    genesis.nVersion = 1;
    genesis.hashPrevBlock = uint256("0000000000000000000000000000000000000000000000000000000000000000");
    genesis.hashMerkleRoot = uint256("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"); // Bitcoin's genesis merkle root
    genesis.nTime = 1737933600; // Our timestamp
    genesis.nBits = 0x1d00ffff;  // Difficulty bits
    genesis.nNonce = 0;
    
    // Target from difficulty bits
    uint256 target("00000000ffff0000000000000000000000000000000000000000000000000000");
    
    std::cout << "Mining CustomCoin genesis block..." << std::endl;
    std::cout << "Target: " << target.GetHex() << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    uint64_t hashes = 0;
    
    while (true) {
        std::string serialized = genesis.Serialize();
        uint256 hash = Hash(serialized);
        
        hashes++;
        
        if (hashes % 100000 == 0) {
            auto now = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - start);
            double rate = hashes / (duration.count() + 1);
            std::cout << "Tried " << hashes << " hashes, rate: " << rate << " H/s, current hash: " << hash.GetHex() << std::endl;
        }
        
        if (hash <= target) {
            std::cout << std::endl << "Found valid genesis block!" << std::endl;
            std::cout << "Nonce: " << genesis.nNonce << std::endl;
            std::cout << "Hash: " << hash.GetHex() << std::endl;
            std::cout << "Total hashes: " << hashes << std::endl;
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
            std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
            
            break;
        }
        
        genesis.nNonce++;
        if (genesis.nNonce == 0) {
            std::cout << "Nonce overflow, incrementing timestamp..." << std::endl;
            genesis.nTime++;
            genesis.nNonce = 0;
        }
    }
    
    return 0;
}