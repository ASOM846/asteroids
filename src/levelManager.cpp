#include "levelManager.hpp"
#include "ui.h"
#include <raymath.h>

LevelManager::LevelManager() {}

LevelManager::~LevelManager() {}

void LevelManager::reset() {
    currentLevel = LevelData{};
    levelRunning = false;
    isLevelCompleted = false;
    levelEnding = false;
    isLevelLose = false;
    progressAccumulator = 0;
    endTimer = 0.0f;
    resetCurrentLevelTime();
    if (ui) {
        ui->resetAll();
    }
}

void LevelManager::runLevel(int levelNumber) {
    if (!levels)
        return;
    for (const auto& level : *levels) {
        if (level.levelNumber == levelNumber) {
            currentLevel = level;
            resetCurrentLevelTime();
            progressAccumulator = 0;
            setLevelRunning(true);

            initLevel(level);

            switch (level.type) {
            case LevelType::SurviveAsteroidField:
                initAsteroidFieldLevel(level);
                std::cout << "Starting Asteroid Field Level " << level.levelNumber
                    << "\n";
                break;
            case LevelType::DestroyAsteroids:
                initDestroyAsteroidsLevel(level);
                std::cout << "Starting Destroy Asteroids Level " << level.levelNumber
                    << "\n";
                break;
            case LevelType::EnemyInvasion:
                initEnemyInvasionLevel(level);
                std::cout << "Starting Enemy Invasion Level " << level.levelNumber
                    << "\n";
                break;
            case LevelType::ShipEscort:
                initShipEscortLevel(level);
                break;
            case LevelType::BossFight:
                // initBossFightLevel(level); --- IGNORE ---
                break;
            default:
                initAsteroidFieldLevel(level);
                break;
            }
            break;
        }
    }
}

void LevelManager::updateCurrentLevel() {
    if (levelEnding) {
        endTimer += GetFrameTime();
        if (endTimer >= endDuration) {
            if (returnToMenuCallback)
                returnToMenuCallback();
            reset();
            levelEnding = false;
        }
        setLevelUnlockedState(currentLevel.levelNumber + 1, true);
        setLevelRunning(false);
        return;
    }

    if (!levelRunning)
        return;

    if (((currentLevelTime >= currentLevel.duration) && currentLevel.duration > 0) ||
        (progressAccumulator >= currentLevel.objectiveCount) && currentLevel.objectiveCount > 0) {
        std::cout << "Level " << currentLevel.levelNumber
            << " completed! Starting end animation...\n";
        levelEnding = true;
        endTimer = 0.0f;
        setLevelRunning(false);
        return;
    }

    currentLevelTime += GetFrameTime();
    switch (currentLevel.type) {
    case LevelType::SurviveAsteroidField:
        updateAsteroidFieldLevel();
        break;
    case LevelType::DestroyAsteroids:
        updateDestroyAsteroidsLevel();
        break;
    case LevelType::EnemyInvasion:
        updateEnemyInvasionLevel();
        break;
    case LevelType::ShipEscort:
        updateShipEscortLevel();
        break;
    case LevelType::BossFight:
        // updateBossFightLevel(); --- IGNORE ---
        break;
    default:
        updateAsteroidFieldLevel();
        break;
    }
}

const LevelData* LevelManager::getCurrentLevelData() const {
    if (!levelRunning && !levelEnding) {
        return nullptr;
    }
    return &currentLevel;
}

int LevelManager::getCurrentLevelNumber() const {
    return currentLevel.levelNumber;
}

int LevelManager::getProgressAccumulator() const { return progressAccumulator; }

float LevelManager::getCurrentLevelTime() const { return currentLevelTime; }

bool LevelManager::isLevelRunning() const { return levelRunning; }

int LevelManager::getUnlockedLevels() const
{
    if (!levels)
        return 1;

    int count = 0;

    for (auto l : *levels) {
        if (l.isUnlocked == 1)
            ++count;
    }

    return count;
}

float LevelManager::getRemainingLevelTime() const {
    return currentLevel.duration - currentLevelTime;
}

void LevelManager::loadLevelsToMemory() {
    LevelData level1;
    level1.type = LevelType::SurviveAsteroidField;
    level1.isUnlocked = true;
    level1.levelNumber = 1;
    level1.objectiveCount = 0;
    level1.duration = 60.0f;
    level1.desiredAsteroidCount = 5;
    level1.desiredEnemiesCount = { {0, 0, 0} };
    level1.objective = "Survive 60 seconds";



    LevelData level2;
    level2.type = LevelType::EnemyInvasion;
    level2.isUnlocked = false;
    level2.levelNumber = 2;
    level2.objectiveCount = 3;
    level2.duration = 60.0f;
    level2.desiredAsteroidCount = 2;
    level2.desiredEnemiesCount = { {2, 0, 0} };
    level2.objective = "Defeat 3 enemies";

    LevelData level3;
    level3.type = LevelType::DestroyAsteroids;
    level3.isUnlocked = false;
    level3.levelNumber = 3;
    level3.objectiveCount = 10;
    level3.duration = 60.0f;
    level3.desiredAsteroidCount = 6;
    level3.desiredEnemiesCount = { {0, 0, 0} };
    level3.objective = "Destroy 10 asteroids";

    LevelData level4;
    level4.type = LevelType::ShipEscort;
    level4.isUnlocked = false;
    level4.levelNumber = 4;
    level4.objectiveCount = 0;
    level4.duration = 0.0f;
    level4.desiredAsteroidCount = 3;
    level4.desiredEnemiesCount = { {0, 0, 0} };
    level4.objective = "Escort ship safely";
    level4.waypoint = { 1000, 1500 };

    LevelData level5;
    level5.type = LevelType::BossFight;
    level5.isUnlocked = false;
    level5.levelNumber = 5;
    level5.objectiveCount = 0;
    level5.duration = 120.0f;
    level5.desiredAsteroidCount = 3;
    level5.desiredEnemiesCount = { {0, 0, 0} };
    level5.objective = "Defeat the Boss \n (work in progress)";

    LevelData level6;
    level6.type = LevelType::EnemyInvasion;
    level6.isUnlocked = false;
    level6.levelNumber = 6;
    level5.objectiveCount = 10;
    level6.duration = 120.0f;
    level6.desiredAsteroidCount = 3;
    level6.desiredEnemiesCount = { {2, 1, 1} };
    level6.objective = "Defeat 15 enemies";

    LevelData level7;
    level7.type = LevelType::SurviveAsteroidField;
    level7.isUnlocked = false;
    level7.levelNumber = 7;
    level7.objectiveCount = 0;
    level7.duration = 120.0f;
    level7.desiredAsteroidCount = 6;
    level7.desiredEnemiesCount = { {2, 0, 0} };
    level7.objective = "Survive 120 seconds";

    LevelData level8;
    level8.type = LevelType::Pursuit;
    level8.isUnlocked = false;
    level8.levelNumber = 8;
    level8.objectiveCount = 0;
    level8.duration = 120.0f;
    level8.desiredAsteroidCount = 4;
    level8.desiredEnemiesCount = { {2, 0, 0} };
    level8.objective = "Pursiut enemy ship";

    levels->push_back(level1);
    levels->push_back(level2);
    levels->push_back(level3);
    levels->push_back(level4);
    levels->push_back(level5);
    levels->push_back(level6);
    levels->push_back(level7);
    levels->push_back(level8);
}

void LevelManager::setLevelUnlockedState(int levelNumber, bool isUnlocked) {
    if (!levels)
        return;
    for (auto& level : *levels) {
        if (level.levelNumber == levelNumber) {
            level.isUnlocked = isUnlocked;
            break;
        }
    }
}

void LevelManager::setUnlockedLevels(int numberOfUnlockedLevels) {
    if (!levels)
        return;
    for (auto& level : *levels) {
        if (level.levelNumber <= numberOfUnlockedLevels)
            level.isUnlocked = true;
    }
}

void LevelManager::drawLevelEndOverlay(int screenWidth, int screenHeight) {
    if (!levelEnding)
        return;

    const char* mainMsg = "Level Completed";

    if (isLevelLose)
        mainMsg = "Level Lose";
            
    float t = endTimer / endDuration;
    if (t > 1.0f)
        t = 1.0f;

    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, t * 0.85f));

    int mainSize = 56;
    int mainW = MeasureText(mainMsg, mainSize);
    DrawText(mainMsg, screenWidth / 2 - mainW / 2, screenHeight / 2 - 40,
        mainSize, GOLD);

    std::string info = "Powrot do menu...";
    if (player && currentLevel.type == LevelType::SurviveAsteroidField) {
        info = "Wynik: " + std::to_string(player->getScore());
    }
    int infoSize = 26;
    int infoW = MeasureText(info.c_str(), infoSize);
    DrawText(info.c_str(), screenWidth / 2 - infoW / 2, screenHeight / 2 + 30,
        infoSize, WHITE);

    int barW = 400;
    int barH = 18;
    int bx = screenWidth / 2 - barW / 2;
    int by = screenHeight / 2 + 70;
    DrawRectangleLines(bx, by, barW, barH, WHITE);
    DrawRectangle(bx + 2, by + 2, (int)((barW - 4) * t), barH - 4, SKYBLUE);
}

void LevelManager::resetCurrentLevelTime() { currentLevelTime = 0.0f; }

void LevelManager::initLevel(const LevelData& level)
{
    if (asteroidHelper)
        asteroidHelper->setAsteroidCount(level.desiredAsteroidCount);

    if (enemyManager)
        enemyManager->setDesiredCounts(level.desiredEnemiesCount);
    
}

void LevelManager::initAsteroidFieldLevel(const LevelData& level) {
    if (level.type != LevelType::SurviveAsteroidField)
        return;

    progressAccumulator = -1;
}

void LevelManager::updateAsteroidFieldLevel() {
    if (currentLevel.type != LevelType::SurviveAsteroidField)
        return;
}

void LevelManager::initDestroyAsteroidsLevel(const LevelData& level) {
    if (currentLevel.type != LevelType::DestroyAsteroids)
        return;
}

void LevelManager::updateDestroyAsteroidsLevel() {
    if (currentLevel.type != LevelType::DestroyAsteroids)
        return;

    progressAccumulator = asteroidHelper->getDestroyedAsteroidCount();
}

void LevelManager::initEnemyInvasionLevel(const LevelData& level) {
    if (currentLevel.type != LevelType::EnemyInvasion)
        return;
}

void LevelManager::updateEnemyInvasionLevel() {
    if (currentLevel.type != LevelType::EnemyInvasion)
        return;

    if (enemyManager->getKilledEnemies() > progressAccumulator)
        progressAccumulator++;
}

void LevelManager::initShipEscortLevel(const LevelData& level) {
    if (currentLevel.type != LevelType::ShipEscort)
        return;


    if (!customShipManager)
        throw std::runtime_error(std::string("customShipManagerNotLoaded"));
    if (!ui)
        throw std::runtime_error(std::string("uiNotLoaded"));

    //zrobic randomowe pocz¹tkowe miejsce w promieniu np 500px i randomowy waypoint np 1500-2000px
    customShipManager->addShip({ -100.0f, GetScreenHeight() / 2.0f }, level.waypoint);

    ui->setArrowDestination(customShipManager->getShipPosition(0));
}

void LevelManager::updateShipEscortLevel() {
    if (currentLevel.type != LevelType::ShipEscort)
        return;

    if (!customShipManager || !ui)
        return;

    if (customShipManager->isEmpty()) {
        isLevelLose = true;
        levelEnding = true;
        endTimer = 0.0f;
        setLevelRunning(false);
        ui->clearArrowDestination();
        return;
    }

    CustomShip* escortShip = customShipManager->getCustomShipPtr(0);
    if (!escortShip) {
        isLevelLose = true;
        levelEnding = true;
        endTimer = 0.0f;
        setLevelRunning(false);
        ui->clearArrowDestination();
        return;
    }

    const Vector2 escortPos = escortShip->position;

    if (!Vector2Equals(escortPos, Vector2{ 0.0f, 0.0f })) {
        ui->setArrowDestination(escortPos);
    }

    if (escortShip->isDestinationReached()) {
        std::printf("DESTINATION REACHED+++++++++++++++++++++++");
        levelEnding = true;
        endTimer = 0.0f;
        setLevelRunning(false);
        return;
    }

    if (!escortShip->bAlive || customShipManager->isEmpty()) {
        isLevelLose = true;
        levelEnding = true;
        endTimer = 0.0f;
        setLevelRunning(false);
    }


}