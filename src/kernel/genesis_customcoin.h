#ifndef BITCOIN_KERNEL_GENESIS_CUSTOMCOIN_H
#define BITCOIN_KERNEL_GENESIS_CUSTOMCOIN_H

#include <amount.h>
#include <primitives/block.h>
#include <uint256.h>

/**
 * CustomCoin Genesis Block Creation Functions
 */

/**
 * Create the CustomCoin genesis block with custom message and parameters
 */
CBlock CreateCustomCoinGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward);

/**
 * Mine a valid nonce for the CustomCoin genesis block
 */
uint32_t MineCustomCoinGenesisBlock(CBlock& genesis, const uint256& powLimit);

#endif // BITCOIN_KERNEL_GENESIS_CUSTOMCOIN_H