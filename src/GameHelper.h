#pragma once
#include <vector>
#include <raylib.h>
#include "customShip.hpp"

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

	float getAngleBetween(const Vector2& from, const Vector2& to) const;
    
	float getAngleBetweenPlayerAndFriendlyShip() const;

private:
    Player* player = nullptr;
	CustomShipManager* customShipManager = nullptr;
};