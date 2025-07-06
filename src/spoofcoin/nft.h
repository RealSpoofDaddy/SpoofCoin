#ifndef SPOOFCOIN_NFT_H
#define SPOOFCOIN_NFT_H

#include <uint256.h>
#include <primitives/transaction.h>
#include <consensus/amount.h>
#include <vector>
#include <memory>
#include <map>
#include <string>

namespace spoofcoin {

/**
 * SpoofCoin Advanced NFT System
 * Next-generation non-fungible tokens with dynamic properties and metaverse integration
 */

class NFTSystem {
public:
    // NFT types and categories
    enum class NFTType {
        STATIC_IMAGE,           // Traditional static image NFT
        DYNAMIC_NFT,            // NFT with changing properties
        GENERATIVE_ART,         // Algorithmically generated art
        INTERACTIVE_NFT,        // Interactive/game-based NFT
        UTILITY_NFT,            // NFT with utility functions
        MEMBERSHIP_NFT,         // Membership/access token
        FRACTIONAL_NFT,         // Fractionalized NFT ownership
        COMPOSABLE_NFT,         // NFT that can be combined
        PROGRAMMABLE_NFT,       // NFT with embedded logic
        METAVERSE_ASSET,        // 3D metaverse asset
        MUSIC_NFT,              // Audio/music NFT
        VIDEO_NFT,              // Video content NFT
        DOMAIN_NFT,             // Domain name NFT
        IDENTITY_NFT,           // Digital identity NFT
        CARBON_CREDIT_NFT       // Environmental carbon credit NFT
    };

    // NFT metadata standards
    enum class MetadataStandard {
        ERC721_COMPATIBLE,      // ERC-721 compatible metadata
        ERC1155_COMPATIBLE,     // ERC-1155 compatible metadata
        SPOOFCOIN_NATIVE,       // Native SpoofCoin NFT standard
        IPFS_METADATA,          // IPFS-based metadata
        ARWEAVE_METADATA,       // Arweave permanent storage
        DYNAMIC_METADATA,       // Dynamic/changing metadata
        ENCRYPTED_METADATA,     // Encrypted metadata
        QUANTUM_METADATA        // Quantum-resistant metadata
    };

    /**
     * NFT Creation and Management
     */
    static bool CreateNFT(NFTType nft_type,
                         const std::string& name,
                         const std::string& description,
                         const std::vector<unsigned char>& metadata,
                         const std::string& creator_address,
                         std::string& nft_id);
    
    static bool MintNFT(const std::string& nft_id,
                       const std::string& recipient_address,
                       uint64_t token_id);
    
    static bool TransferNFT(const std::string& nft_id,
                          uint64_t token_id,
                          const std::string& from_address,
                          const std::string& to_address);
    
    static bool BurnNFT(const std::string& nft_id,
                       uint64_t token_id,
                       const std::string& owner_address);

    /**
     * Dynamic NFT Properties
     */
    static bool UpdateNFTMetadata(const std::string& nft_id,
                                uint64_t token_id,
                                const std::vector<unsigned char>& new_metadata);
    
    static bool AddDynamicProperty(const std::string& nft_id,
                                 uint64_t token_id,
                                 const std::string& property_name,
                                 const std::string& property_value);
    
    static bool UpdateDynamicProperty(const std::string& nft_id,
                                    uint64_t token_id,
                                    const std::string& property_name,
                                    const std::string& new_value);
    
    static bool SetPropertyUpdateRule(const std::string& nft_id,
                                    const std::string& property_name,
                                    const std::vector<unsigned char>& update_logic);

    /**
     * Fractionalized NFT Ownership
     */
    static bool FractionalizeNFT(const std::string& nft_id,
                                uint64_t token_id,
                                uint64_t total_fractions,
                                const std::string& owner_address,
                                std::string& fraction_token_id);
    
    static bool TransferFractions(const std::string& fraction_token_id,
                                uint64_t fraction_amount,
                                const std::string& from_address,
                                const std::string& to_address);
    
    static bool RedeemFractions(const std::string& fraction_token_id,
                              uint64_t fraction_amount,
                              const std::string& redeemer_address);
    
    static bool VoteOnFractionalizedNFT(const std::string& fraction_token_id,
                                       const std::string& proposal_id,
                                       bool vote_yes,
                                       uint64_t voting_power,
                                       const std::string& voter_address);

    /**
     * NFT Royalty Management
     */
    static bool SetRoyaltyInfo(const std::string& nft_id,
                             const std::string& royalty_recipient,
                             double royalty_percentage);
    
    static bool UpdateRoyaltyRecipient(const std::string& nft_id,
                                     const std::string& new_recipient);
    
    static bool DistributeRoyalties(const std::string& nft_id,
                                  CAmount sale_amount,
                                  const std::string& seller_address);
    
    static bool SetSecondaryRoyalty(const std::string& nft_id,
                                  const std::string& secondary_recipient,
                                  double secondary_percentage);

    /**
     * NFT Marketplace Integration
     */
    static bool ListNFTForSale(const std::string& nft_id,
                             uint64_t token_id,
                             CAmount price,
                             const std::string& seller_address,
                             std::string& listing_id);
    
    static bool PurchaseNFT(const std::string& listing_id,
                          const std::string& buyer_address,
                          CAmount payment_amount);
    
    static bool CreateNFTAuction(const std::string& nft_id,
                               uint64_t token_id,
                               CAmount starting_price,
                               int64_t auction_duration,
                               const std::string& seller_address,
                               std::string& auction_id);
    
    static bool PlaceBid(const std::string& auction_id,
                       CAmount bid_amount,
                       const std::string& bidder_address);
    
    static bool FinalizeAuction(const std::string& auction_id);

    /**
     * Metaverse Integration
     */
    static bool Create3DAsset(const std::string& nft_id,
                            const std::vector<unsigned char>& model_data,
                            const std::vector<unsigned char>& texture_data,
                            const std::string& format); // GLTF, FBX, etc.
    
    static bool SetMetaverseProperties(const std::string& nft_id,
                                     const std::map<std::string, std::string>& properties);
    
    static bool EnableMetaverseInteraction(const std::string& nft_id,
                                         const std::vector<unsigned char>& interaction_script);
    
    static bool RegisterInMetaverse(const std::string& nft_id,
                                  const std::string& metaverse_platform,
                                  const std::string& world_coordinates);

    /**
     * Composable NFTs
     */
    static bool CreateComposableNFT(const std::string& parent_nft_id,
                                  const std::vector<std::string>& child_nft_ids,
                                  const std::string& composition_rules,
                                  std::string& composable_nft_id);
    
    static bool AttachChildNFT(const std::string& parent_nft_id,
                             const std::string& child_nft_id,
                             const std::string& attachment_point);
    
    static bool DetachChildNFT(const std::string& parent_nft_id,
                             const std::string& child_nft_id);
    
    static bool EvolveComposableNFT(const std::string& composable_nft_id,
                                  const std::vector<unsigned char>& evolution_data);

public:
    // NFT data structures (made public for collection access)
    struct NFTMetadata {
        std::string nft_id;
        NFTType nft_type;
        std::string name;
        std::string description;
        std::vector<unsigned char> metadata;
        std::string creator_address;
        MetadataStandard metadata_standard;
        std::map<std::string, std::string> dynamic_properties;
        std::map<std::string, std::vector<unsigned char>> property_update_rules;
        int64_t creation_time;
        bool is_dynamic;
        bool is_fractionalized;
        bool is_composable;
    };

    struct NFTOwnership {
        std::string nft_id;
        uint64_t token_id;
        std::string owner_address;
        bool is_fractional;
        std::string fraction_token_id;
        uint64_t fraction_amount;
        int64_t ownership_time;
    };

    struct RoyaltyInfo {
        std::string nft_id;
        std::string primary_recipient;
        double primary_percentage;
        std::string secondary_recipient;
        double secondary_percentage;
        CAmount total_royalties_paid;
    };

    struct MarketplaceListing {
        std::string listing_id;
        std::string nft_id;
        uint64_t token_id;
        std::string seller_address;
        CAmount price;
        bool is_auction;
        std::string auction_id;
        int64_t listing_time;
        bool active;
    };

    struct NFTAuction {
        std::string auction_id;
        std::string nft_id;
        uint64_t token_id;
        std::string seller_address;
        CAmount starting_price;
        CAmount current_highest_bid;
        std::string highest_bidder;
        int64_t start_time;
        int64_t end_time;
        bool finalized;
        std::vector<std::pair<std::string, CAmount>> bid_history;
    };

    static std::map<std::string, NFTMetadata> nft_metadata_;
    static std::map<std::string, std::vector<NFTOwnership>> nft_ownership_;
    static std::map<std::string, RoyaltyInfo> royalty_info_;
    static std::map<std::string, MarketplaceListing> marketplace_listings_;
    static std::map<std::string, NFTAuction> nft_auctions_;
    static uint64_t next_nft_id_;
    static uint64_t next_token_id_;

private:
    // Internal implementation details
};

/**
 * Advanced NFT Collections
 */
class NFTCollection {
public:
    // Collection types
    enum class CollectionType {
        STANDARD_COLLECTION,    // Standard NFT collection
        GENERATIVE_COLLECTION,  // Algorithmically generated collection
        COLLABORATIVE_COLLECTION, // Community-created collection
        EVOLVING_COLLECTION,    // Collection that evolves over time
        INTERACTIVE_COLLECTION, // Interactive/game-based collection
        UTILITY_COLLECTION,     // Collection with utility functions
        MEMBERSHIP_COLLECTION,  // Membership-based collection
        CHARITY_COLLECTION,     // Charity/fundraising collection
        METAVERSE_COLLECTION,   // Metaverse-specific collection
        EDUCATIONAL_COLLECTION  // Educational/learning collection
    };

    /**
     * Collection Management
     */
    static bool CreateCollection(CollectionType collection_type,
                               const std::string& name,
                               const std::string& description,
                               const std::string& creator_address,
                               uint64_t max_supply,
                               std::string& collection_id);
    
    static bool AddNFTToCollection(const std::string& collection_id,
                                 const std::string& nft_id);
    
    static bool RemoveNFTFromCollection(const std::string& collection_id,
                                      const std::string& nft_id);
    
    static bool SetCollectionRoyalty(const std::string& collection_id,
                                   const std::string& royalty_recipient,
                                   double royalty_percentage);

    /**
     * Generative Collections
     */
    static bool CreateGenerativeCollection(const std::string& collection_id,
                                         const std::vector<unsigned char>& generation_algorithm,
                                         const std::map<std::string, std::vector<std::string>>& traits);
    
    static bool GenerateNFT(const std::string& collection_id,
                          const std::string& recipient_address,
                          std::string& generated_nft_id);
    
    static bool SetGenerationParameters(const std::string& collection_id,
                                      const std::map<std::string, double>& parameters);

    /**
     * Collection Analytics
     */
    static uint64_t GetCollectionSize(const std::string& collection_id);
    static CAmount GetCollectionFloorPrice(const std::string& collection_id);
    static CAmount GetCollectionVolume(const std::string& collection_id);
    static double GetCollectionRarity(const std::string& collection_id, const std::string& nft_id);
    static std::map<std::string, uint64_t> GetTraitDistribution(const std::string& collection_id);

private:
    struct NFTCollectionData {
        std::string collection_id;
        CollectionType collection_type;
        std::string name;
        std::string description;
        std::string creator_address;
        uint64_t max_supply;
        uint64_t current_supply;
        std::vector<std::string> nft_ids;
        std::map<std::string, std::vector<std::string>> traits;
        std::vector<unsigned char> generation_algorithm;
        std::map<std::string, double> generation_parameters;
        int64_t creation_time;
        bool is_generative;
        NFTSystem::RoyaltyInfo royalty_info;
    };

    static std::map<std::string, NFTCollectionData> collections_;
    static uint64_t next_collection_id_;
};

/**
 * NFT Gaming Integration
 */
class NFTGaming {
public:
    // Gaming NFT types
    enum class GameNFTType {
        CHARACTER,              // Game character NFT
        WEAPON,                 // Weapon/tool NFT
        ARMOR,                  // Armor/equipment NFT
        CONSUMABLE,             // Consumable item NFT
        LAND,                   // Virtual land NFT
        BUILDING,               // Building/structure NFT
        VEHICLE,                // Vehicle NFT
        PET,                    // Pet/companion NFT
        ACHIEVEMENT,            // Achievement/badge NFT
        QUEST_ITEM,             // Quest-specific item NFT
        COSMETIC,               // Cosmetic/appearance NFT
        SKILL_CARD,             // Skill/ability card NFT
        RESOURCE,               // Resource/material NFT
        BLUEPRINT              // Crafting blueprint NFT
    };

    /**
     * Gaming NFT Management
     */
    static bool CreateGameNFT(GameNFTType game_nft_type,
                            const std::string& name,
                            const std::map<std::string, double>& stats,
                            const std::string& game_id,
                            const std::string& creator_address,
                            std::string& game_nft_id);
    
    static bool UpgradeGameNFT(const std::string& game_nft_id,
                             const std::map<std::string, double>& stat_improvements,
                             const std::vector<std::string>& required_materials);
    
    static bool CombineGameNFTs(const std::vector<std::string>& source_nft_ids,
                              const std::vector<unsigned char>& combination_recipe,
                              std::string& result_nft_id);
    
    static bool BreakdownGameNFT(const std::string& game_nft_id,
                                std::vector<std::string>& component_nft_ids);

    /**
     * Cross-Game Compatibility
     */
    static bool RegisterGameNFTForCrossGame(const std::string& game_nft_id,
                                          const std::vector<std::string>& compatible_games);
    
    static bool ConvertNFTForGame(const std::string& game_nft_id,
                                const std::string& target_game_id,
                                std::string& converted_nft_id);
    
    static bool ValidateNFTCompatibility(const std::string& game_nft_id,
                                       const std::string& target_game_id);

    /**
     * Gaming Marketplace
     */
    static bool CreateGameItemMarketplace(const std::string& game_id,
                                        const std::string& marketplace_name,
                                        std::string& marketplace_id);
    
    static bool ListGameNFTForSale(const std::string& marketplace_id,
                                 const std::string& game_nft_id,
                                 CAmount price,
                                 const std::string& seller_address);
    
    static bool CreateGameNFTRental(const std::string& game_nft_id,
                                  CAmount rental_price_per_day,
                                  int64_t max_rental_duration,
                                  const std::string& owner_address,
                                  std::string& rental_id);

private:
    struct GameNFTData {
        std::string game_nft_id;
        GameNFTType game_nft_type;
        std::string name;
        std::map<std::string, double> stats;
        std::string game_id;
        std::vector<std::string> compatible_games;
        std::string creator_address;
        std::string current_owner;
        uint64_t level;
        uint64_t experience;
        std::vector<std::string> upgrade_history;
        bool is_tradeable;
        bool is_rentable;
        int64_t creation_time;
    };

    struct GameNFTRental {
        std::string rental_id;
        std::string game_nft_id;
        std::string owner_address;
        std::string renter_address;
        CAmount rental_price_per_day;
        int64_t rental_start_time;
        int64_t rental_end_time;
        bool active;
    };

    static std::map<std::string, GameNFTData> game_nfts_;
    static std::map<std::string, GameNFTRental> nft_rentals_;
    static std::map<std::string, std::vector<std::string>> game_marketplaces_;
};

/**
 * NFT Analytics and Insights
 */
class NFTAnalytics {
public:
    /**
     * Market Analytics
     */
    static CAmount CalculateNFTValue(const std::string& nft_id, uint64_t token_id);
    static double CalculateNFTRarity(const std::string& nft_id, uint64_t token_id);
    static std::vector<CAmount> GetNFTPriceHistory(const std::string& nft_id, uint64_t token_id);
    static double PredictNFTValue(const std::string& nft_id, uint64_t token_id, int64_t future_time);
    
    /**
     * Collection Analytics
     */
    static std::map<std::string, uint64_t> GetCollectionTraitRarity(const std::string& collection_id);
    static CAmount GetCollectionAveragePrice(const std::string& collection_id);
    static double GetCollectionVolatility(const std::string& collection_id);
    static std::vector<std::string> GetTrendingCollections(uint64_t limit);
    
    /**
     * Owner Analytics
     */
    static std::vector<std::string> GetOwnerNFTPortfolio(const std::string& owner_address);
    static CAmount GetOwnerPortfolioValue(const std::string& owner_address);
    static double GetOwnerTradingProfitability(const std::string& owner_address);
    static std::vector<std::string> GetOwnerTradingHistory(const std::string& owner_address);

private:
    struct NFTAnalyticsData {
        std::string nft_id;
        uint64_t token_id;
        std::vector<CAmount> price_history;
        std::vector<int64_t> sale_timestamps;
        std::vector<std::string> previous_owners;
        double rarity_score;
        CAmount estimated_value;
        double volatility;
        std::map<std::string, std::string> traits;
    };

    static std::map<std::string, NFTAnalyticsData> analytics_data_;
    static void UpdateAnalyticsData(const std::string& nft_id, uint64_t token_id, CAmount sale_price);
};

} // namespace spoofcoin

#endif // SPOOFCOIN_NFT_H