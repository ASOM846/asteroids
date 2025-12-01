#pragma once
#include <vector>
#include <raylib.h>
#include "laser.h"
#include "asteroid.h"
#include "drop.h"
#include "enemy.hpp"

class TextureManager;
class Player;

class GameHelper {
public:
    GameHelper();
    ~GameHelper();

    void checkForFullScreen();
    void setDropChance(float c) { dropSpawnChance = c; }

    void handleCollision(std::vector<Laser>& lasers,
        std::vector<sAsteroid>& asteroids,
        const Rectangle& playerRect);
    void checkCollisionPlayerDrop(std::vector<sDrop>& drops,
        Rectangle playerRect);
    void checkCollisionLaserPlayerEnemy();
    void setTextures(TextureManager &textureManager, Player &player);

    void setPlayerHealthPtr(int* ptr);
    void setPlayerShieldPtr(int* ptr) { playerShieldPtr = ptr; }
    void setDropHelper(DropHelper* helper) { dropHelper = helper; }
    void setPlayer(Player* pPlayer) { player = pPlayer; }
    void setDrops(std::vector<sDrop>* d) { drops = d; }
    void setPlayingTime(float arg) { playingTime = arg; }

    void setPointers(std::vector<sEnemy>* enemyList,
        std::vector<Laser>* laserList) {
        enemies = enemyList;
        lasers = laserList;
    }

    void setCamera(Camera2D* camera);

    float getPlayingTime() { return playingTime; }

    void drawPosition();
    void handleGameTiming();
private:
    int* playerHealthPtr = nullptr;
    int* playerShieldPtr = nullptr;
    DropHelper* dropHelper = nullptr;
    Player* player = nullptr;
    std::vector<sDrop>* drops = nullptr;
    std::vector<sEnemy>* enemies = nullptr;
    std::vector<Laser>* lasers = nullptr;

    float dropSpawnChance = 0.30f;

    void maybeSpawnDrop(float x, float y);

    float playingTime;
};