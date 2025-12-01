#include "levelManager.hpp"

LevelManager::LevelManager() {}

LevelManager::~LevelManager() {}

void LevelManager::runLevel(int levelNumber) {
    if (!levels) return;
    for (const auto& level : *levels) {
        if (level.levelNumber == levelNumber) {
            currentLevel = level;
            switch (level.type) {
            case LevelType::SurviveAsteroidField:
                initAsteroidFieldLevel(level);
                std::cout << "Starting Asteroid Field Level " << level.levelNumber << "\n";
                break;
            case LevelType::EnemyInvasion:
                //initEnemyInvasionLevel(level); --- IGNORE ---
                break;
            case LevelType::ShipEscort:
                //initShipEscortLevel(level); --- IGNORE ---
                break;
            case LevelType::BossFight:
                //initBossFightLevel(level); --- IGNORE ---
                break;
            default:
                initAsteroidFieldLevel(level);
                break;
            }
            break;
        }
    }
}

LevelData LevelManager::getCurrentLevelData() {
    return currentLevel;
}

int LevelManager::getCurrentLevelNumber() const {
    return currentLevel.levelNumber;
}

void LevelManager::loadLevelsToMemory() {
    LevelData level1;
    level1.levelNumber = 1;
    level1.difficulty = 1;
    level1.type = LevelType::SurviveAsteroidField;
    level1.duration = 60.0f;

    LevelData level2;
    level2.levelNumber = 2;
    level2.difficulty = 1;
    level2.type = LevelType::EnemyInvasion;
    level2.duration = 60.0f;

    levels->push_back(level1);
    levels->push_back(level2);
}

void LevelManager::initAsteroidFieldLevel(LevelData level)  {
    if(level.type != LevelType::SurviveAsteroidField) return;

    asteroidHelper->setAsteroidCount(level.difficulty * 10);    
}
