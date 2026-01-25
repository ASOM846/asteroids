#pragma once
#include <vector>
#include <raylib.h>
#include <cmath>
#include "laser.h"
#include "asteroid.h"
#include "drop.h"
#include "enemy.hpp"
#include "ui.h"
#include "GameHelper.h"
#include "customShip.hpp"
#include "player.h"
#include "cameraManager.hpp"

// Forward declarations
class Player;
class Game;

class CollisionSystem {
public:
    CollisionSystem() = default;
    ~CollisionSystem() = default;

    void setPointers(std::vector<Laser>* pLasers,
                     std::vector<sAsteroid>* pAsteroids,
                     std::vector<sDrop>* pDrops,
                     std::vector<sEnemy>* pEnemies,
                     std::vector<CustomShip>* pCustomShips,
                     Player* pPlayer,
                     DropHelper* pDropHelper,
                     GameHelper* pGameHelper,
                     CameraManager* pCameraManager,
                     Game* pGame) {
        lasers = pLasers;
        asteroids = pAsteroids;
        drops = pDrops;
        enemies = pEnemies;
        customShips = pCustomShips;
        player = pPlayer;
        dropHelper = pDropHelper;
        gameHelper = pGameHelper;
        cameraManager = pCameraManager;
        game = pGame;
    }

    void handleCollision();

private:
    std::vector<Laser>* lasers = nullptr;
    std::vector<sAsteroid>* asteroids = nullptr;
    std::vector<sDrop>* drops = nullptr;
    std::vector<sEnemy>* enemies = nullptr;
    std::vector<CustomShip>* customShips = nullptr;
    Player* player = nullptr;
    DropHelper* dropHelper = nullptr;
    Game* game = nullptr;
    GameHelper* gameHelper = nullptr;
    CameraManager* cameraManager = nullptr;
};