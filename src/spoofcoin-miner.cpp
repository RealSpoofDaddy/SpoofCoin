#include <chainparams.h>
#include <common/args.h>
#include <logging.h>
#include <spoofcoin/miner.h>
#include <spoofcoin/security.h>
#include <util/chaintype.h>
#include <util/strencodings.h>
#include <util/translation.h>
#include <script/script.h>

#include <iostream>
#include <memory>
#include <signal.h>
#include <string>
#include <thread>
#include <chrono>

const std::function<std::string(const char*)> G_TRANSLATION_FUN = nullptr;

/**
 * SpoofCoin Advanced Miner
 * Standalone mining application with enhanced features
 */

static std::unique_ptr<spoofcoin::AdvancedMiner> g_miner;
static bool g_running = true;

static void SignalHandler(int signal)
{
    std::cout << "\nReceived signal " << signal << ", shutting down...\n";
    g_running = false;
    if (g_miner) {
        g_miner->StopMining();
    }
}

static void PrintUsage()
{
    std::cout << "SpoofCoin Advanced Miner v2.0\n";
    std::cout << "Usage: spoofcoin-miner [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -?                     This help message\n";
    std::cout << "  -chain=<chain>         Use the chain <chain> (default: spoofcoin)\n";
    std::cout << "  -threads=<n>           Number of mining threads (default: auto)\n";
    std::cout << "  -address=<address>     Mining address for rewards\n";
    std::cout << "  -benchmark             Run mining benchmark\n";
    std::cout << "  -stats                 Show mining statistics periodically\n";
    std::cout << "  -config=<file>         Configuration file path\n";
    std::cout << "  -datadir=<dir>         Specify data directory\n";
    std::cout << "  -debug                 Enable debug logging\n\n";
    std::cout << "Examples:\n";
    std::cout << "  spoofcoin-miner -address=sc1qexample... -threads=4\n";
    std::cout << "  spoofcoin-miner -benchmark\n";
    std::cout << "  spoofcoin-miner -chain=spoofcoin -stats\n\n";
}

static void PrintMiningStats(const spoofcoin::AdvancedMiner::MiningStats::Snapshot& stats)
{
    std::cout << "\n==== SpoofCoin Miner Statistics ====\n";
    std::cout << "Status: " << (stats.is_mining ? "MINING" : "STOPPED") << "\n";
    std::cout << "Hashes Computed: " << stats.hashes_computed << "\n";
    std::cout << "Blocks Found: " << stats.blocks_found << "\n";
    std::cout << "Hash Rate: " << stats.hash_rate << " H/s\n";
    std::cout << "Runtime: " << (GetTime() - stats.start_time) << " seconds\n";
    std::cout << "=====================================\n\n";
}

static bool RunBenchmark()
{
    std::cout << "Running SpoofCoin miner benchmark...\n";
    
    try {
        SelectParams(ChainType::SPOOFCOIN);
        const auto& chainparams = Params();
        
        spoofcoin::AdvancedMiner miner(chainparams);
        spoofcoin::AdvancedMiner::MiningConfig config = spoofcoin::MiningUtils::OptimizeMiningConfig();
        
        // Set benchmark address - create a simple P2PKH script
        config.mining_address = CScript() << OP_DUP << OP_HASH160 << std::vector<unsigned char>(20, 0) << OP_EQUALVERIFY << OP_CHECKSIG;
        
        std::cout << "Starting benchmark with " << config.threads << " threads...\n";
        
        if (!miner.StartMining(config)) {
            std::cerr << "Failed to start mining for benchmark\n";
            return false;
        }
        
        // Run benchmark for 30 seconds
        std::this_thread::sleep_for(std::chrono::seconds(30));
        
        auto stats = miner.GetStats();
        miner.StopMining();
        
        std::cout << "\n==== Benchmark Results ====\n";
        std::cout << "Duration: 30 seconds\n";
        std::cout << "Threads: " << config.threads << "\n";
        std::cout << "Total Hashes: " << stats.hashes_computed << "\n";
        std::cout << "Average Hash Rate: " << stats.hash_rate << " H/s\n";
        std::cout << "Estimated Daily Blocks: " << spoofcoin::MiningUtils::CalculateBlocksPerDay(stats.hash_rate) << "\n";
        std::cout << "==========================\n";
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Benchmark error: " << e.what() << "\n";
        return false;
    }
}

static bool SetupMiner(spoofcoin::AdvancedMiner::MiningConfig& config)
{
    // Set default values
    config = spoofcoin::MiningUtils::OptimizeMiningConfig();
    
    // Parse command line arguments
    if (gArgs.IsArgSet("-threads")) {
        config.threads = gArgs.GetIntArg("-threads", config.threads);
        if (config.threads <= 0) {
            std::cerr << "Invalid thread count\n";
            return false;
        }
    }
    
    if (gArgs.IsArgSet("-address")) {
        std::string address = gArgs.GetArg("-address", "");
        if (address.empty()) {
            std::cerr << "Mining address is required\n";
            return false;
        }
        
        // Create a simple P2PKH script from the address string
        // In production, this would properly parse and validate the address
        std::vector<unsigned char> addr_bytes(address.begin(), address.end());
        if (addr_bytes.size() > 20) addr_bytes.resize(20);
        while (addr_bytes.size() < 20) addr_bytes.push_back(0);
        
        config.mining_address = CScript() << OP_DUP << OP_HASH160 << addr_bytes << OP_EQUALVERIFY << OP_CHECKSIG;
    } else {
        std::cerr << "Mining address is required. Use -address=<address>\n";
        return false;
    }
    
    std::cout << "Mining Configuration:\n";
    std::cout << "  Threads: " << config.threads << "\n";
    std::cout << "  Mining Address: " << gArgs.GetArg("-address", "") << "\n";
    std::cout << "  CPU Mining: " << (config.use_cpu_mining ? "Yes" : "No") << "\n";
    std::cout << "  Smart Fee Selection: " << (config.smart_fee_selection ? "Yes" : "No") << "\n\n";
    
    return true;
}

int main(int argc, char* argv[])
{
    // Setup signal handlers
    signal(SIGINT, SignalHandler);
    signal(SIGTERM, SignalHandler);
    
    try {
        std::string error;
        if (!gArgs.ParseParameters(argc, argv, error)) {
            std::cerr << "Error parsing parameters: " << error << "\n";
            return 1;
        }
        
        if (gArgs.IsArgSet("-?") || gArgs.IsArgSet("-help")) {
            PrintUsage();
            return 0;
        }
        
        // Setup basic logging
        if (gArgs.GetBoolArg("-debug", false)) {
            LogInstance().m_print_to_console = true;
        }
        
        // Select chain
        std::string chain = gArgs.GetArg("-chain", "spoofcoin");
        if (chain != "spoofcoin") {
            std::cerr << "SpoofCoin miner only supports -chain=spoofcoin\n";
            return 1;
        }
        
        SelectParams(ChainType::SPOOFCOIN);
        const auto& chainparams = Params();
        
        LogPrintf("SpoofCoin Miner starting for chain: %s\n", chain);
        
        // Handle benchmark mode
        if (gArgs.GetBoolArg("-benchmark", false)) {
            return RunBenchmark() ? 0 : 1;
        }
        
        // Setup miner configuration
        spoofcoin::AdvancedMiner::MiningConfig config;
        if (!SetupMiner(config)) {
            return 1;
        }
        
        // Create and start miner
        g_miner = std::make_unique<spoofcoin::AdvancedMiner>(chainparams);
        
        std::cout << "Starting SpoofCoin Advanced Miner...\n";
        std::cout << "Press Ctrl+C to stop\n\n";
        
        if (!g_miner->StartMining(config)) {
            std::cerr << "Failed to start mining\n";
            return 1;
        }
        
        // Mining loop with periodic statistics
        bool show_stats = gArgs.GetBoolArg("-stats", true);
        auto last_stats_time = std::chrono::steady_clock::now();
        
        while (g_running && g_miner->IsMining()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            
            if (show_stats) {
                auto now = std::chrono::steady_clock::now();
                if (std::chrono::duration_cast<std::chrono::seconds>(now - last_stats_time).count() >= 30) {
                    PrintMiningStats(g_miner->GetStats());
                    last_stats_time = now;
                }
            }
        }
        
        std::cout << "Stopping miner...\n";
        g_miner->StopMining();
        
        // Final statistics
        PrintMiningStats(g_miner->GetStats());
        
        std::cout << "SpoofCoin miner stopped successfully\n";
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred\n";
        return 1;
    }
}