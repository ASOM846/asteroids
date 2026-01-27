#pragma once
#include <raylib.h>
#include "levels.hpp"

class LevelGenerator {
    public:
        static LevelData generateLevel(const int difficultyLevel = 1);
    private:
        static LevelType randomLevelType();
        static int randomObjectiveCount(const LevelType& type, const int difficulty);
        static float randomDuration(const LevelType& type);
        static int randomAsteroidCount(const LevelType& level, const float difficulty);
        static std::array<int, 3> randomEnemyCounts(const LevelType& level, const float difficulty);
        static std::string generateObjectiveString(const LevelData& level);
};

inline LevelData LevelGenerator::generateLevel(const int difficultyLevel) {
    LevelData level;
    level.type = randomLevelType();
    level.isUnlocked = true;
    level.levelNumber = -1;
    level.objectiveCount = randomObjectiveCount(level.type, difficultyLevel);
    level.duration = randomDuration(level.type);
    level.desiredAsteroidCount = randomAsteroidCount(level.type, static_cast<float>(difficultyLevel));
    level.desiredEnemiesCount = randomEnemyCounts(level.type, static_cast<float>(difficultyLevel));
    level.waypoint = { static_cast<float>(GetRandomValue(800, 2000)), 
        static_cast<float>(GetRandomValue(100, GetScreenHeight() - 100)) };
    level.objective = generateObjectiveString(level);
    return level;
}

inline LevelType LevelGenerator::randomLevelType() {
    const int typeIndex = GetRandomValue(
        static_cast<int>(LevelType::SurviveAsteroidField),
        static_cast<int>(LevelType::ShipEscort)
    );
    return static_cast<LevelType>(typeIndex);
}

inline int LevelGenerator::randomObjectiveCount(const LevelType& type, const int difficulty) {
    switch (type) {
        case LevelType::SurviveAsteroidField:
            return 0;
        case LevelType::DestroyAsteroids:
            return GetRandomValue(5, 15) * difficulty;
        case LevelType::EnemyInvasion:
            return GetRandomValue(3, 10) * difficulty;
        case LevelType::ShipEscort:
            return 0;
        // case LevelType::BossFight:
        //     return 1;
        // case LevelType::ProtectBase:
        //     return GetRandomValue(1, 5);
        // case LevelType::Pursuit:
        //     return 0;
        default:
            return 0;
    }
}

inline float LevelGenerator::randomDuration(const LevelType& type) {
    if(type == LevelType::SurviveAsteroidField) 
        return GetRandomValue(1.5, 12) * 10;
    return -1;
}

inline int LevelGenerator::randomAsteroidCount(const LevelType& level, const float difficulty) {
    switch (level) {
        case LevelType::SurviveAsteroidField:
            return GetRandomValue(3, 10) * difficulty;
        case LevelType::DestroyAsteroids:
            return GetRandomValue(4, 6) * difficulty;
        case LevelType::EnemyInvasion:
            return GetRandomValue(1, 4) * difficulty;
        case LevelType::ShipEscort:
            return GetRandomValue(1, 3) * difficulty;
        // case LevelType::BossFight:
        //     return 0;
        // case LevelType::ProtectBase:
        //     return GetRandomValue(1, 3);
        // case LevelType::Pursuit:
        //     return GetRandomValue(1, 4);
        default:
            return 0;
    }
}

inline std::array<int, 3> LevelGenerator::randomEnemyCounts(const LevelType& level, const float difficulty) {
    std::array<int, 3> enemyCounts = {0, 0, 0};
    switch (level) {
        case LevelType::SurviveAsteroidField:
            break;
        case LevelType::DestroyAsteroids:
            break;
        case LevelType::EnemyInvasion:
            enemyCounts[0] = GetRandomValue(2, 5) * difficulty;
            if(difficulty > 1.0f) {
                enemyCounts[1] = GetRandomValue(1, 3) * (difficulty - 1);
            }
            if(difficulty > 2.0f) {
                enemyCounts[2] = GetRandomValue(1, 2) * (difficulty - 2);
            }
            break;
        case LevelType::ShipEscort:
            if(difficulty > 1.0f) {
                enemyCounts[0] = GetRandomValue(1, 3) * (difficulty - 1);
            }
            break;
        // case LevelType::BossFight:
        //     enemyCounts[2] = 1;
        //     break;
        // case LevelType::ProtectBase:
        //     enemyCounts[0] = GetRandomValue(1, 3);
        //     break;
        // case LevelType::Pursuit:
        //     enemyCounts[1] = GetRandomValue(1, 4);
        //     break;
        default:
            break;
    }
    return enemyCounts;
}

inline std::string LevelGenerator::generateObjectiveString(const LevelData& level) {
    switch (level.type) {
        case LevelType::SurviveAsteroidField:
            return "Survive for " + std::to_string(static_cast<int>(level.duration)) + " seconds";
        case LevelType::DestroyAsteroids:
            return "Destroy " + std::to_string(level.objectiveCount) + " asteroids";
        case LevelType::EnemyInvasion:
            return "Defeat " + std::to_string(level.objectiveCount) + " enemies";
        case LevelType::ShipEscort:
            return "Escort the ship safely";
        // case LevelType::BossFight:
        //     return "Defeat the Boss";
        // case LevelType::ProtectBase:
        //     return "Protect your base from enemies";
        // case LevelType::Pursuit:
        //     return "Pursue and destroy the enemy ship";
        default:
            return "Complete the objective";
    }
}