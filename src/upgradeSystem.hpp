#pragma once
#include <string>
#include <vector>
#include <map>

/**
 * @file upgradeSystem.hpp
 * @brief Persistent upgrade system for player ship enhancements
 * 
 * This system manages permanent upgrades that persist across game sessions.
 * Players can purchase upgrades using earned currency to improve their ship's
 * stats including health, shield, damage, fire rate, and mobility.
 */

enum class UpgradeType {
    MaxHealth,
    MaxShield,
    MaxAmmo,
    FireRate,
    Damage,
    Speed,
    TurnSpeed,
    ShieldRegenRate
};

struct Upgrade {
    UpgradeType type;
    std::string name;
    std::string description;
    int level;
    int maxLevel;
    int baseCost;
    
    int getCurrentCost() const {
        return baseCost * (level + 1);
    }
    
    bool canUpgrade() const {
        return level < maxLevel;
    }
};

class UpgradeSystem {
public:
    UpgradeSystem();
    ~UpgradeSystem() = default;
    
    void initializeUpgrades();
    
    bool purchaseUpgrade(UpgradeType type, int& currency);
    int getUpgradeLevel(UpgradeType type) const;
    const Upgrade* getUpgrade(UpgradeType type) const;
    const std::vector<Upgrade>& getAllUpgrades() const { return upgrades; }
    
    // Get stat modifiers based on upgrade levels
    int getMaxHealthBonus() const;
    int getMaxShieldBonus() const;
    int getMaxAmmoBonus() const;
    float getFireRateMultiplier() const;
    float getDamageMultiplier() const;
    float getSpeedMultiplier() const;
    float getTurnSpeedMultiplier() const;
    float getShieldRegenRate() const;
    
    // Serialization
    void saveToString(std::string& data) const;
    void loadFromString(const std::string& data);
    
private:
    std::vector<Upgrade> upgrades;
    std::map<UpgradeType, size_t> upgradeIndexMap;
    
    void addUpgrade(UpgradeType type, const std::string& name, 
                    const std::string& desc, int maxLvl, int baseCost);
};
