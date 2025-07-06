#ifndef SPOOFCOIN_MINER_H
#define SPOOFCOIN_MINER_H

#include <primitives/block.h>
#include <uint256.h>
#include <util/time.h>
#include <atomic>
#include <thread>
#include <memory>

class CChainParams;
class CBlockIndex;
class CScript;

namespace spoofcoin {

/**
 * SpoofCoin Advanced Miner
 * Features: Multi-threaded mining, adaptive difficulty, smart block assembly
 */
class AdvancedMiner {
public:
    struct MiningStats {
        std::atomic<uint64_t> hashes_computed{0};
        std::atomic<uint64_t> blocks_found{0};
        std::atomic<double> hash_rate{0.0};
        std::atomic<int64_t> start_time{0};
        std::atomic<bool> is_mining{false};
        
        // Make non-copyable to avoid atomic copy issues
        MiningStats() = default;
        MiningStats(const MiningStats&) = delete;
        MiningStats& operator=(const MiningStats&) = delete;
        
        // Allow creation of snapshot for reading
        struct Snapshot {
            uint64_t hashes_computed;
            uint64_t blocks_found;
            double hash_rate;
            int64_t start_time;
            bool is_mining;
        };
        
        Snapshot GetSnapshot() const {
            return {
                hashes_computed.load(),
                blocks_found.load(),
                hash_rate.load(),
                start_time.load(),
                is_mining.load()
            };
        }
    };

    struct MiningConfig {
        int threads = 1;
        bool use_cpu_mining = true;
        bool adaptive_difficulty = true;
        bool smart_fee_selection = true;
        CScript mining_address;
        int64_t max_block_weight = 4000000;
        int64_t min_tx_fee = 1000; // satoshis
    };

    AdvancedMiner(const CChainParams& chainparams);
    ~AdvancedMiner();

    /**
     * Start mining with specified configuration
     */
    bool StartMining(const MiningConfig& config);

    /**
     * Stop mining
     */
    void StopMining();

    /**
     * Get current mining statistics snapshot
     */
    MiningStats::Snapshot GetStats() const { return stats_.GetSnapshot(); }

    /**
     * Check if currently mining
     */
    bool IsMining() const { return stats_.is_mining.load(); }

    /**
     * Get estimated time to find next block
     */
    int64_t GetEstimatedTimeToBlock() const;

    /**
     * Get current hash rate
     */
    double GetHashRate() const { return stats_.hash_rate.load(); }

private:
    /**
     * Main mining loop for a single thread
     */
    void MiningLoop(int thread_id);

    /**
     * Create a block template for mining
     */
    std::unique_ptr<CBlock> CreateBlockTemplate();

    /**
     * Assemble transactions for the block
     */
    void AssembleTransactions(CBlock& block);

    /**
     * Mine a single block
     */
    bool MineBlock(CBlock& block, uint32_t& nonce_out);

    /**
     * Validate and submit a found block
     */
    bool SubmitBlock(const CBlock& block);

    /**
     * Update mining statistics
     */
    void UpdateStats();

    /**
     * Adaptive difficulty adjustment for optimal mining
     */
    void AdjustMiningDifficulty();

    /**
     * Smart transaction selection based on fees and priority
     */
    void SelectOptimalTransactions(CBlock& block);

    const CChainParams& chainparams_;
    MiningConfig config_;
    MiningStats stats_;
    std::vector<std::thread> mining_threads_;
    std::atomic<bool> stop_mining_{false};
    std::atomic<uint32_t> current_nonce_{0};
};

/**
 * SpoofCoin Mining Pool Interface
 */
class MiningPool {
public:
    struct PoolConfig {
        std::string pool_address;
        uint16_t pool_port;
        std::string worker_name;
        std::string worker_password;
        bool use_stratum = true;
    };

    /**
     * Connect to mining pool
     */
    bool ConnectToPool(const PoolConfig& config);

    /**
     * Start pool mining
     */
    bool StartPoolMining();

    /**
     * Stop pool mining
     */
    void StopPoolMining();

    /**
     * Submit share to pool
     */
    bool SubmitShare(const CBlock& block, uint32_t nonce);

private:
    PoolConfig pool_config_;
    bool connected_ = false;
};

/**
 * SpoofCoin CPU Miner Optimizations
 */
class CPUMiner {
public:
    /**
     * Optimized SHA256 hashing for SpoofCoin
     */
    static uint256 OptimizedSHA256(const CBlock& block, uint32_t nonce);

    /**
     * SIMD-optimized mining function
     */
    static bool MineWithSIMD(CBlock& block, uint32_t start_nonce, uint32_t end_nonce, uint32_t& found_nonce);

    /**
     * Cache-optimized mining loop
     */
    static void CacheOptimizedMining(CBlock& block, uint32_t& nonce, const uint256& target);

    /**
     * Get optimal thread count for current CPU
     */
    static int GetOptimalThreadCount();
};

/**
 * SpoofCoin Mining Utilities
 */
class MiningUtils {
public:
    /**
     * Calculate mining profitability
     */
    static double CalculateProfitability(double hash_rate, double power_consumption, double electricity_cost);

    /**
     * Estimate mining rewards
     */
    static CAmount EstimateMiningReward(int64_t blocks_per_day);

    /**
     * Get network difficulty
     */
    static double GetNetworkDifficulty();

    /**
     * Calculate expected blocks per day
     */
    static double CalculateBlocksPerDay(double hash_rate);

    /**
     * Optimize mining parameters
     */
    static AdvancedMiner::MiningConfig OptimizeMiningConfig();
};

} // namespace spoofcoin

#endif // SPOOFCOIN_MINER_H