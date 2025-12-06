#pragma once
#include <vector>
#include <raylib.h>
#include "laser.h"
#include "asteroid.h"
#include "drop.h"
#include "enemy.hpp"
#include "player.h"
#include <iostream>
#include <functional> // <-- added
#include <string>     // <-- optional, useful for drawing text

enum class LevelType {
    SurviveAsteroidField,
    DestroyAsteroids,
    EnemyInvasion,
    ShipEscort,
    BossFight
};

struct LevelData {
    int levelNumber;
    int difficulty;
    float duration;
    std::string objective;
    int objectiveCount;
    int currentCount;
    LevelType type;
    Vector2 waypoint;
};

class LevelManager {
public:
    LevelManager();
    ~LevelManager();

    void reset();

    void runLevel(int levelNumber);
    void updateCurrentLevel();
    LevelData getCurrentLevelData();
    int getCurrentLevelNumber() const;
    float getCurrentLevelTime() const;
    float getRemainingLevelTime() const;
    bool isLevelRunning() const;

    void setOnLevelComplete(std::function<void()> cb) { onLevelComplete = cb; }

    void drawLevelEndOverlay(int screenWidth, int screenHeight);

    bool isEnding() const { return levelEnding; }

    void setPointers(std::vector<LevelData>* levelVec,
        DropHelper* dh,
        Player* p,
        AsteroidHelper* ah,
        std::vector<sDrop>* dropVec,
        std::vector<sEnemy>* enemyVec) {
        levels = levelVec;
        dropHelper = dh;
        player = p;
        asteroidHelper = ah;
        drops = dropVec;
        enemies = enemyVec;
    }

    void loadLevelsToMemory();
private:

    LevelData currentLevel;

    std::vector<LevelData>* levels;
    DropHelper *dropHelper = nullptr;
    Player *player = nullptr;
    AsteroidHelper *asteroidHelper = nullptr;
    std::vector<sDrop> *drops = nullptr;
    std::vector<sEnemy> *enemies = nullptr;

    bool levelRunning = false;
    float currentLevelTime = 0.0f;

    void resetCurrentLevelTime();
    void setLevelRunning(bool running) { levelRunning = running; }

    void initAsteroidFieldLevel(const LevelData &level);
    void updateAsteroidFieldLevel();

    std::function<void()> onLevelComplete;
    bool levelEnding = false;
    float endTimer = 0.0f;
    float endDuration = 2.0f;
};