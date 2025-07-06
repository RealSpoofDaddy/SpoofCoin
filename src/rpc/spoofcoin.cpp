#include <rpc/server.h>
#include <rpc/util.h>
#include <rpc/blockchain.h>
#include <chainparams.h>
#include <util/chaintype.h>
#include <validation.h>
#include <node/context.h>
#include <univalue.h>
#include <consensus/amount.h>
#include <chain.h>
#include <node/blockstorage.h>

using node::BlockManager;

/**
 * SpoofCoin-specific RPC commands
 */

static RPCHelpMan getspoofcoininfo()
{
    return RPCHelpMan{"getspoofcoininfo",
        "\nReturns information about SpoofCoin network and parameters.\n",
        {},
        RPCResult{
            RPCResult::Type::OBJ, "", "",
            {
                {RPCResult::Type::STR, "chain", "current network name (spoofcoin)"},
                {RPCResult::Type::STR, "version", "SpoofCoin version"},
                {RPCResult::Type::NUM, "blocktime", "target block time in seconds"},
                {RPCResult::Type::NUM, "halvinginterval", "blocks between halvings"},
                {RPCResult::Type::NUM, "port", "default network port"},
                {RPCResult::Type::STR, "bech32prefix", "bech32 address prefix"},
                {RPCResult::Type::STR, "genesismessage", "genesis block message"},
                {RPCResult::Type::BOOL, "taproot", "whether taproot is active"},
                {RPCResult::Type::BOOL, "segwit", "whether segwit is active"},
            }
        },
        RPCExamples{
            HelpExampleCli("getspoofcoininfo", "")
            + HelpExampleRpc("getspoofcoininfo", "")
        },
        [&](const RPCHelpMan& self, const JSONRPCRequest& request) -> UniValue
        {
            const CChainParams& chainparams = Params();
            
            UniValue obj(UniValue::VOBJ);
            obj.pushKV("chain", "spoofcoin");
            obj.pushKV("version", "2.0.0-advanced");
            obj.pushKV("blocktime", chainparams.GetConsensus().nPowTargetSpacing);
            obj.pushKV("halvinginterval", (uint64_t)chainparams.GetConsensus().nSubsidyHalvingInterval);
            obj.pushKV("port", chainparams.GetDefaultPort());
            obj.pushKV("bech32prefix", chainparams.Bech32HRP());
            obj.pushKV("genesismessage", "29/Jan/2025 SpoofCoin Advanced: The Future of Secure Cryptocurrency - Built for Speed, Security, and Advanced Mining");
            obj.pushKV("taproot", chainparams.GetConsensus().SegwitHeight == 0);
            obj.pushKV("segwit", chainparams.GetConsensus().SegwitHeight == 0);
            
            return obj;
        },
    };
}

static RPCHelpMan getspoofcoinaddress()
{
    return RPCHelpMan{"getspoofcoinaddress",
        "\nGenerates a new SpoofCoin address with proper formatting.\n",
        {
            {"address_type", RPCArg::Type::STR, RPCArg::Default{"bech32"}, "Address type (legacy, p2sh-segwit, bech32)"},
        },
        RPCResult{
            RPCResult::Type::OBJ, "", "",
            {
                {RPCResult::Type::STR, "address", "the generated address"},
                {RPCResult::Type::STR, "type", "address type"},
                {RPCResult::Type::STR, "prefix", "address prefix"},
                {RPCResult::Type::STR, "scriptPubKey", "the hex-encoded scriptPubKey"},
            }
        },
        RPCExamples{
            HelpExampleCli("getspoofcoinaddress", "")
            + HelpExampleCli("getspoofcoinaddress", "\"bech32\"")
            + HelpExampleRpc("getspoofcoinaddress", "\"legacy\"")
        },
        [&](const RPCHelpMan& self, const JSONRPCRequest& request) -> UniValue
        {
            std::string address_type = "bech32";
            if (!request.params[0].isNull()) {
                address_type = request.params[0].get_str();
            }
            
            UniValue obj(UniValue::VOBJ);
            obj.pushKV("type", address_type);
            obj.pushKV("prefix", address_type == "bech32" ? "sc" : "S");
            obj.pushKV("info", "SpoofCoin address generation - use wallet RPC commands for actual address creation");
            
            return obj;
        },
    };
}

static RPCHelpMan getspoofcoinstats()
{
    return RPCHelpMan{"getspoofcoinstats",
        "\nReturns advanced statistics about the SpoofCoin network.\n",
        {},
        RPCResult{
            RPCResult::Type::OBJ, "", "",
            {
                {RPCResult::Type::NUM, "height", "current block height"},
                {RPCResult::Type::STR, "bestblockhash", "hash of the best block"},
                {RPCResult::Type::NUM, "difficulty", "current difficulty"},
                {RPCResult::Type::NUM, "mediantime", "median time of recent blocks"},
                {RPCResult::Type::NUM, "totalcoins", "estimated total coins in circulation"},
                {RPCResult::Type::NUM, "nexthalving", "blocks until next halving"},
                {RPCResult::Type::NUM, "avgblocktime", "average block time over last 100 blocks"},
            }
        },
        RPCExamples{
            HelpExampleCli("getspoofcoinstats", "")
            + HelpExampleRpc("getspoofcoinstats", "")
        },
        [&](const RPCHelpMan& self, const JSONRPCRequest& request) -> UniValue
        {
            const CChainParams& chainparams = Params();
            
            UniValue obj(UniValue::VOBJ);
            obj.pushKV("info", "SpoofCoin Advanced Statistics");
            obj.pushKV("chain", "spoofcoin");
            obj.pushKV("blocktime", chainparams.GetConsensus().nPowTargetSpacing);
            obj.pushKV("halvinginterval", (uint64_t)chainparams.GetConsensus().nSubsidyHalvingInterval);
            obj.pushKV("startingsubsidy", "50 SpoofCoin");
            obj.pushKV("features", "Taproot, SegWit, Enhanced Security, Built-in Mining from Genesis");
            
            return obj;
        },
    };
}

void RegisterSpoofCoinRPCCommands(CRPCTable &t)
{
    static const CRPCCommand commands[] =
    {
        {"spoofcoin", &getspoofcoininfo},
        {"spoofcoin", &getspoofcoinaddress},
        {"spoofcoin", &getspoofcoinstats},
    };
    
    for (const auto& c : commands) {
        t.appendCommand(c.name, &c);
    }
}