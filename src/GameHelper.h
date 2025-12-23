#pragma once
#include <vector>
#include <raylib.h>
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

    void setTextures(TextureManager &textureManager, Player &player);

    void setPlayer(Player* pPlayer) { player = pPlayer; }
	void setCustomShipManager(CustomShipManager* pCsm) { customShipManager = pCsm; }

    void setCamera(Camera2D* camera);
    void triggerShake(float intensity, float duration);
    void triggerShake();
    void updateShake(float dt);

	float getAngleBetween(const Vector2& from, const Vector2& to) const;
    
	float getAngleBetweenPlayerAndFriendlyShip() const;

private:
    Player* player = nullptr;
    Camera2D* camera = nullptr;
	CustomShipManager* customShipManager = nullptr;

    ScreenShake screenShake;
};