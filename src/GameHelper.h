#pragma once
#include <vector>
#include <raylib.h>
#include "laser.h"
#include "asteroid.h"
#include "drop.h"
#include "enemy.hpp"
#include "ui.h"
#include "customShip.hpp"
#include "shakeScreen.hpp"

class TextureManager;
class Player;
struct ShakeScreen;

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
    void triggerShake(float intensity, float duration);
    void triggerShake();
    void updateShake(float dt);
    void drawPosition();

	float getAngleBetween(const Vector2& from, const Vector2& to) const;
    
	float getAngleBetweenPlayerAndFriendlyShip() const;

private:
    int* playerHealthPtr = nullptr;
    int* playerShieldPtr = nullptr;
    DropHelper* dropHelper = nullptr;
    Player* player = nullptr;
	Ui* ui = nullptr;
    Camera2D* camera = nullptr;
	CustomShipManager* customShipManager = nullptr;
    std::vector<sDrop>* drops = nullptr;
    std::vector<sEnemy>* enemies = nullptr;
    std::vector<Laser>* lasers = nullptr;

    ScreenShake screenShake;
};