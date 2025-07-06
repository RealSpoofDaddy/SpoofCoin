#include <kernel/genesis_customcoin.h>
#include <arith_uint256.h>
#include <chainparams.h>
#include <consensus/merkle.h>
#include <hash.h>
#include <primitives/block.h>
#include <primitives/transaction.h>
#include <pubkey.h>
#include <script/script.h>
#include <uint256.h>
#include <util/strencodings.h>

/**
 * CustomCoin Genesis Block Generator
 * Creates a meaningful genesis block with custom message and parameters
 */

static CBlock CreateCustomCoinGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

CBlock CreateCustomCoinGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "29/Jan/2025 CustomCoin: The Future of Decentralized Finance - Built for Speed, Security, and Scalability";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateCustomCoinGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

uint32_t MineCustomCoinGenesisBlock(CBlock& genesis, const uint256& powLimit)
{
    printf("Mining CustomCoin Genesis Block...\n");
    
    arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
    if (hashTarget > UintToArith256(powLimit)) {
        hashTarget = UintToArith256(powLimit);
    }

    uint32_t nNonce = 0;
    while (true) {
        genesis.nNonce = nNonce;
        uint256 hash = genesis.GetHash();
        
        if (UintToArith256(hash) <= hashTarget) {
            printf("CustomCoin Genesis Block Found!\n");
            printf("Hash: %s\n", hash.ToString().c_str());
            printf("Nonce: %u\n", nNonce);
            printf("Merkle Root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
            return nNonce;
        }
        
        nNonce++;
        if (nNonce % 10000 == 0) {
            printf("Nonce: %u, Hash: %s\n", nNonce, hash.ToString().c_str());
        }
    }
}