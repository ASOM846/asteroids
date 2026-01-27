#pragma once
#include <array>
#include <string>
#include <raylib.h>

enum class LevelType {
    SurviveAsteroidField,
    DestroyAsteroids,
    EnemyInvasion,
    ShipEscort,
    BossFight,//not working
    ProtectBase,//not working
    Pursuit//not working
};

inline const char* LevelTypeToString(LevelType t) noexcept {
    switch (t) {
    case LevelType::SurviveAsteroidField: return "Asteroid Field";
    case LevelType::DestroyAsteroids:     return "Destroy Asteroids";
    case LevelType::EnemyInvasion:        return "Enemy Invasion";
    case LevelType::ShipEscort:           return "Ship Escort";
    case LevelType::BossFight:            return "Boss Fight";
    case LevelType::ProtectBase:          return "Protect Base";
    case LevelType::Pursuit:              return "Pursuit";
    default:                              return "Unknown";
    }
}

struct LevelData {
    LevelType type;
    bool isUnlocked;
    int levelNumber;
    int objectiveCount;
    float duration;
    int desiredAsteroidCount;
    std::array<int, 3> desiredEnemiesCount;
    std::string objective;
    Vector2 waypoint;
};