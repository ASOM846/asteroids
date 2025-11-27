#pragma once
#include <vector>
#include <raylib.h>
#include "laser.h"
#include "asteroid.h"
#include "drop.h"

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

    void setTextures(TextureManager& textureManager, Player& player);

    void setPlayerHealthPtr(int* ptr);
    void setPlayerShieldPtr(int* ptr) { playerShieldPtr = ptr; }
    void setDropHelper(DropHelper* helper) { dropHelper = helper; }
    void setPlayer(Player* pPlayer) { player = pPlayer; }
    void setDrops(std::vector<sDrop>* d) { drops = d; }

private:
    int* playerHealthPtr = nullptr;
    int* playerShieldPtr = nullptr;
    DropHelper* dropHelper = nullptr;
    Player* player = nullptr;
    std::vector<sDrop>* drops = nullptr;
    float dropSpawnChance = 0.30f;

    void maybeSpawnDrop(float x, float y);
};