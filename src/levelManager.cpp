#include "levelManager.hpp"
#include "ui.h"
#include <raymath.h>

LevelManager::LevelManager() {}

LevelManager::~LevelManager() {}

void LevelManager::reset() {
    currentLevel = LevelData{};
    levelRunning = false;
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

            levelLogic.initLevel(level);
        }
    }
}

void LevelManager::updateCurrentLevel() {

    if(IsKeyPressed(KEY_M)) {
        if(currentMode == Mode::Endless)
            currentMode = Mode::Levels;
        else
            currentMode = Mode::Endless;
    }

    if(currentMode == Mode::Endless)
        DrawText("Endless Mode", 10, 10, 20, GREEN);

    if (currentLevelResult == LevelUpdateResult::Completed || 
        currentLevelResult == LevelUpdateResult::Failed) {
        
        switch (currentMode) {
        case Mode::Endless:
            currentLevel = LevelGenerator::generateLevel(1);
            currentLevelResult = LevelUpdateResult::None;
            resetCurrentLevelTime();
            progressAccumulator = 0;
            levelLogic.initLevel(currentLevel);
            break;
        case Mode::Levels:

            endTimer += GetFrameTime();
            if (endTimer >= endDuration) {
                if (returnToMenuCallback)
                    returnToMenuCallback();
                reset();
                currentLevelResult = LevelUpdateResult::None;
            }
            setLevelUnlockedState(currentLevel.levelNumber + 1, true);
            setLevelRunning(false);
        break;
        }
        return;
    }

    if (!levelRunning)
        return;

    if (((currentLevelTime >= currentLevel.duration) && currentLevel.duration > 0) ||
        ((progressAccumulator >= currentLevel.objectiveCount) && currentLevel.objectiveCount > 0)) {
        std::cout << "Level " << currentLevel.levelNumber
            << " completed! Starting end animation...\n";
        currentLevelResult = LevelUpdateResult::Completed;
        endTimer = 0.0f;
        setLevelRunning(false);
        return;
    }

    currentLevelTime += GetFrameTime();

	currentLevelResult = levelLogic.update(currentLevel);
}

const LevelData* LevelManager::getCurrentLevelData() const {
    if (!levelRunning) {
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
	level6.objectiveCount = 3;
	level6.duration = 60.0f;
	level6.desiredAsteroidCount = 2;
	level6.desiredEnemiesCount = {{2, 0, 0}};
	level6.objective = "Defeat 3 enemies";

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

	LevelData level9;
	level9.type = LevelType::ShipEscort;
	level9.isUnlocked = false;
	level9.levelNumber = 9;
	level9.objectiveCount = 0;
	level9.duration = 0.0f;
	level9.desiredAsteroidCount = 3;
	level9.desiredEnemiesCount = {{0, 0, 0}};
	level9.objective = "Escort ship safely";
	level9.waypoint = {1000, 1500};

	LevelData level10;
	level10.type = LevelType::ShipEscort;
	level10.isUnlocked = false;
	level10.levelNumber = 10;
	level10.objectiveCount = 0;
	level10.duration = 0.0f;
	level10.desiredAsteroidCount = 3;
	level10.desiredEnemiesCount = {{0, 0, 0}};
	level10.objective = "Escort ship safely";
	level10.waypoint = {1000, 1500};

	levels->push_back(level1);
    levels->push_back(level2);
    levels->push_back(level3);
    levels->push_back(level4);
    levels->push_back(level5);
    levels->push_back(level6);
    levels->push_back(level7);
    levels->push_back(level8);

    levels->push_back(level9);
    levels->push_back(level10);
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
    if (currentLevelResult == LevelUpdateResult::None)
        return;

    const char* mainMsg = "Level Completed";

    if (currentLevelResult == LevelUpdateResult::Failed)
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