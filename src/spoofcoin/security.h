#ifndef SPOOFCOIN_SECURITY_H
#define SPOOFCOIN_SECURITY_H

#include <uint256.h>
#include <primitives/block.h>
#include <primitives/transaction.h>
#include <util/time.h>

// Forward declarations
class CBlockIndex;

namespace spoofcoin {

/**
 * SpoofCoin Enhanced Security Features
 */

class SecurityManager {
public:
    /**
     * Advanced block validation with anti-spam measures
     */
    static bool ValidateBlockSecurity(const CBlock& block, const CBlockIndex* pindexPrev);
    
    /**
     * Enhanced transaction validation
     */
    static bool ValidateTransactionSecurity(const CTransaction& tx);
    
    /**
     * Anti-replay attack protection
     */
    static bool CheckReplayProtection(const CTransaction& tx);
    
    /**
     * Mining difficulty manipulation detection
     */
    static bool CheckDifficultyManipulation(const CBlockIndex* pindex);
    
    /**
     * Advanced timestamp validation
     */
    static bool ValidateBlockTimestamp(const CBlock& block, const CBlockIndex* pindexPrev);
    
    /**
     * Network spam prevention
     */
    static bool CheckNetworkSpam(const CBlock& block);
    
    /**
     * Enhanced signature validation
     */
    static bool ValidateEnhancedSignatures(const CTransaction& tx);
    
private:
    static constexpr int64_t MAX_BLOCK_TIME_DRIFT = 2 * 60 * 60; // 2 hours
    static constexpr size_t MAX_BLOCK_SIZE_ENHANCED = 4 * 1000 * 1000; // 4MB
    static constexpr int MIN_CONFIRMATIONS_ENHANCED = 6;
};

/**
 * SpoofCoin Anti-Fork Protection
 */
class ForkProtection {
public:
    /**
     * Detect suspicious fork attempts
     */
    static bool DetectForkAttack(const CBlockIndex* pindex);
    
    /**
     * Chain reorganization protection
     */
    static bool ValidateReorganization(const CBlockIndex* pindexNew, const CBlockIndex* pindexFork);
    
    /**
     * Selfish mining detection
     */
    static bool DetectSelfishMining(const CBlockIndex* pindex);
};

/**
 * SpoofCoin Network Security
 */
class NetworkSecurity {
public:
    /**
     * Enhanced peer validation
     */
    static bool ValidatePeerSecurity(const std::string& peer_address);
    
    /**
     * DDoS protection
     */
    static bool CheckDDoSProtection(const std::string& peer_address);
    
    /**
     * Sybil attack detection
     */
    static bool DetectSybilAttack(const std::vector<std::string>& peer_addresses);
    
    /**
     * Eclipse attack prevention
     */
    static bool PreventEclipseAttack();
};

} // namespace spoofcoin

#endif // SPOOFCOIN_SECURITY_H