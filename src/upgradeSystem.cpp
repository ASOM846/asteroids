#include "upgradeSystem.hpp"
#include <sstream>
#include <iostream>

UpgradeSystem::UpgradeSystem() {
    initializeUpgrades();
}

void UpgradeSystem::initializeUpgrades() {
    upgrades.clear();
    upgradeIndexMap.clear();
    
    addUpgrade(UpgradeType::MaxHealth, "Max Health", 
               "Increase maximum health", 10, 50);
    addUpgrade(UpgradeType::MaxShield, "Max Shield", 
               "Increase maximum shield", 10, 60);
    addUpgrade(UpgradeType::MaxAmmo, "Max Ammo", 
               "Increase maximum ammo capacity", 10, 40);
    addUpgrade(UpgradeType::FireRate, "Fire Rate", 
               "Increase fire rate", 5, 80);
    addUpgrade(UpgradeType::Damage, "Damage", 
               "Increase weapon damage", 5, 100);
    addUpgrade(UpgradeType::Speed, "Speed", 
               "Increase ship speed", 5, 70);
    addUpgrade(UpgradeType::TurnSpeed, "Turn Speed", 
               "Increase turn speed", 5, 50);
    addUpgrade(UpgradeType::ShieldRegenRate, "Shield Regen", 
               "Shields regenerate over time", 5, 120);
}

void UpgradeSystem::addUpgrade(UpgradeType type, const std::string& name, 
                                const std::string& desc, int maxLvl, int baseCost) {
    Upgrade upgrade;
    upgrade.type = type;
    upgrade.name = name;
    upgrade.description = desc;
    upgrade.level = 0;
    upgrade.maxLevel = maxLvl;
    upgrade.baseCost = baseCost;
    
    upgradeIndexMap[type] = upgrades.size();
    upgrades.push_back(upgrade);
}

bool UpgradeSystem::purchaseUpgrade(UpgradeType type, int& currency) {
    auto it = upgradeIndexMap.find(type);
    if (it == upgradeIndexMap.end()) {
        return false;
    }
    
    Upgrade& upgrade = upgrades[it->second];
    
    if (!upgrade.canUpgrade()) {
        return false;
    }
    
    int cost = upgrade.getCurrentCost();
    if (currency < cost) {
        return false;
    }
    
    currency -= cost;
    upgrade.level++;
    return true;
}

int UpgradeSystem::getUpgradeLevel(UpgradeType type) const {
    auto it = upgradeIndexMap.find(type);
    if (it == upgradeIndexMap.end()) {
        return 0;
    }
    return upgrades[it->second].level;
}

const Upgrade* UpgradeSystem::getUpgrade(UpgradeType type) const {
    auto it = upgradeIndexMap.find(type);
    if (it == upgradeIndexMap.end()) {
        return nullptr;
    }
    return &upgrades[it->second];
}

int UpgradeSystem::getMaxHealthBonus() const {
    return getUpgradeLevel(UpgradeType::MaxHealth) * 20;
}

int UpgradeSystem::getMaxShieldBonus() const {
    return getUpgradeLevel(UpgradeType::MaxShield) * 20;
}

int UpgradeSystem::getMaxAmmoBonus() const {
    return getUpgradeLevel(UpgradeType::MaxAmmo) * 50;
}

float UpgradeSystem::getFireRateMultiplier() const {
    int level = getUpgradeLevel(UpgradeType::FireRate);
    return 1.0f + (level * 0.15f); // 15% faster per level
}

float UpgradeSystem::getDamageMultiplier() const {
    int level = getUpgradeLevel(UpgradeType::Damage);
    return 1.0f + (level * 0.2f); // 20% more damage per level
}

float UpgradeSystem::getSpeedMultiplier() const {
    int level = getUpgradeLevel(UpgradeType::Speed);
    return 1.0f + (level * 0.1f); // 10% faster per level
}

float UpgradeSystem::getTurnSpeedMultiplier() const {
    int level = getUpgradeLevel(UpgradeType::TurnSpeed);
    return 1.0f + (level * 0.15f); // 15% faster turning per level
}

float UpgradeSystem::getShieldRegenRate() const {
    int level = getUpgradeLevel(UpgradeType::ShieldRegenRate);
    if (level == 0) return 0.0f;
    return level * 2.0f; // 2 shield per second per level
}

void UpgradeSystem::saveToString(std::string& data) const {
    std::ostringstream oss;
    for (const auto& upgrade : upgrades) {
        oss << static_cast<int>(upgrade.type) << ":" << upgrade.level << ";";
    }
    data = oss.str();
}

void UpgradeSystem::loadFromString(const std::string& data) {
    if (data.empty()) return;
    
    std::istringstream iss(data);
    std::string token;
    
    while (std::getline(iss, token, ';')) {
        if (token.empty()) continue;
        
        size_t colonPos = token.find(':');
        if (colonPos == std::string::npos) continue;
        
        try {
            int typeInt = std::stoi(token.substr(0, colonPos));
            int level = std::stoi(token.substr(colonPos + 1));
            
            // Validate enum value is in range
            if (typeInt < 0 || typeInt >= static_cast<int>(UpgradeType::ShieldRegenRate) + 1) {
                std::cerr << "Invalid upgrade type in save data: " << typeInt << std::endl;
                continue;
            }
            
            UpgradeType type = static_cast<UpgradeType>(typeInt);
            auto it = upgradeIndexMap.find(type);
            if (it != upgradeIndexMap.end()) {
                upgrades[it->second].level = level;
            }
        } catch (...) {
            std::cerr << "Error parsing upgrade data" << std::endl;
        }
    }
}
