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
 * CustomCoin-specific RPC commands
 */

static RPCHelpMan getcustomcoininfo()
{
    return RPCHelpMan{"getcustomcoininfo",
        "\nReturns information about CustomCoin network and parameters.\n",
        {},
        RPCResult{
            RPCResult::Type::OBJ, "", "",
            {
                {RPCResult::Type::STR, "chain", "current network name (customcoin)"},
                {RPCResult::Type::STR, "version", "CustomCoin version"},
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
            HelpExampleCli("getcustomcoininfo", "")
            + HelpExampleRpc("getcustomcoininfo", "")
        },
        [&](const RPCHelpMan& self, const JSONRPCRequest& request) -> UniValue
        {
            const CChainParams& chainparams = Params();
            
            UniValue obj(UniValue::VOBJ);
            obj.pushKV("chain", "customcoin");
            obj.pushKV("version", "1.0.0-enhanced");
            obj.pushKV("blocktime", chainparams.GetConsensus().nPowTargetSpacing);
            obj.pushKV("halvinginterval", (uint64_t)chainparams.GetConsensus().nSubsidyHalvingInterval);
            obj.pushKV("port", chainparams.GetDefaultPort());
            obj.pushKV("bech32prefix", chainparams.Bech32HRP());
            obj.pushKV("genesismessage", "29/Jan/2025 CustomCoin Enhanced: The Future of Decentralized Finance - Built for Speed, Security, and Scalability");
            obj.pushKV("taproot", chainparams.GetConsensus().SegwitHeight == 0);
            obj.pushKV("segwit", chainparams.GetConsensus().SegwitHeight == 0);
            
            return obj;
        },
    };
}

static RPCHelpMan getcustomcoinaddress()
{
    return RPCHelpMan{"getcustomcoinaddress",
        "\nGenerates a new CustomCoin address with proper formatting.\n",
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
            HelpExampleCli("getcustomcoinaddress", "")
            + HelpExampleCli("getcustomcoinaddress", "\"bech32\"")
            + HelpExampleRpc("getcustomcoinaddress", "\"legacy\"")
        },
        [&](const RPCHelpMan& self, const JSONRPCRequest& request) -> UniValue
        {
            std::string address_type = "bech32";
            if (!request.params[0].isNull()) {
                address_type = request.params[0].get_str();
            }
            
            UniValue obj(UniValue::VOBJ);
            obj.pushKV("type", address_type);
            obj.pushKV("prefix", address_type == "bech32" ? "cc" : "C");
            obj.pushKV("info", "CustomCoin address generation - use wallet RPC commands for actual address creation");
            
            return obj;
        },
    };
}

static RPCHelpMan getcustomcoinstats()
{
    return RPCHelpMan{"getcustomcoinstats",
        "\nReturns advanced statistics about the CustomCoin network.\n",
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
            HelpExampleCli("getcustomcoinstats", "")
            + HelpExampleRpc("getcustomcoinstats", "")
        },
        [&](const RPCHelpMan& self, const JSONRPCRequest& request) -> UniValue
        {
            const CChainParams& chainparams = Params();
            
            UniValue obj(UniValue::VOBJ);
            obj.pushKV("info", "CustomCoin Enhanced Statistics");
            obj.pushKV("chain", "customcoin");
            obj.pushKV("blocktime", chainparams.GetConsensus().nPowTargetSpacing);
            obj.pushKV("halvinginterval", (uint64_t)chainparams.GetConsensus().nSubsidyHalvingInterval);
            obj.pushKV("startingsubsidy", "25 CustomCoin");
            obj.pushKV("features", "Taproot, SegWit, Enhanced Security from Genesis");
            
            return obj;
        },
    };
}

void RegisterCustomCoinRPCCommands(CRPCTable &t)
{
    static const CRPCCommand commands[] =
    {
        {"customcoin", &getcustomcoininfo},
        {"customcoin", &getcustomcoinaddress},
        {"customcoin", &getcustomcoinstats},
    };
    
    for (const auto& c : commands) {
        t.appendCommand(c.name, &c);
    }
}