#pragma once
#include <vector>
#include <raylib.h>
#include "laser.h"
#include "asteroid.h"
#include "drop.h"
#include "enemy.hpp"
#include "ui.h"
#include "customShip.hpp"

class TextureManager;
class Player;

class GameHelper {
public:
    GameHelper();
    ~GameHelper();

    void checkForFullScreen();

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
	void setUi(Ui* pUi) { ui = pUi; }
	void setCustomShipManager(CustomShipManager* pCsm) { customShipManager = pCsm; }

    void setPointers(std::vector<sEnemy>* enemyList,
        std::vector<Laser>* laserList) {
        enemies = enemyList;
        lasers = laserList;
    }

    void setCamera(Camera2D* camera);

    void drawPosition();

	//returns angle in radians between two positions
	float getAngleBetween(const Vector2& from, const Vector2& to) const;
    
	//returns angle in radians between player and friendly ship with ID 1
	float getAngleBetweenPlayerAndFriendlyShip() const;

private:
    int* playerHealthPtr = nullptr;
    int* playerShieldPtr = nullptr;
    DropHelper* dropHelper = nullptr;
    Player* player = nullptr;
	Ui* ui = nullptr;
	CustomShipManager* customShipManager = nullptr;
    std::vector<sDrop>* drops = nullptr;
    std::vector<sEnemy>* enemies = nullptr;
    std::vector<Laser>* lasers = nullptr;
};