#pragma once
#include <vector>
#include <raylib.h>
#include "laser.h"
#include "asteroid.h"
#include "drop.h"
#include "enemy.hpp"
#include "player.h"
#include <iostream>

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
    
    void runLevel(int levelNumber);
    LevelData getCurrentLevelData();
    int getCurrentLevelNumber() const;

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


    void initAsteroidFieldLevel(LevelData level);
};