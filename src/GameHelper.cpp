#include "GameHelper.h"
#include "textureManager.h"
#include "player.h"
#include <cmath>
#include <algorithm>

GameHelper::GameHelper() = default;
GameHelper::~GameHelper() = default;

void GameHelper::checkForFullScreen() {
    if (IsKeyPressed(KEY_F11)) {
        ToggleFullscreen();
    }
}

void GameHelper::setTextures(TextureManager& textureManager,
    Player& player) {
    player.setTexture(textureManager.get(TextureId::Player));
}

float GameHelper::getAngleBetween(const Vector2& from, const Vector2& to) const {
    Vector2 delta{ to.x - from.x, to.y - from.y };
    return std::atan2(delta.y, delta.x);
}

float GameHelper::getAngleBetweenPlayerAndFriendlyShip() const {
    if (!player || !customShipManager) return 0.0f;

    Vector2 cachedPlayerPos = player->getPosition();
    Vector2 cachedFriendlyShipPos = customShipManager->getShipPosition(1);

    return getAngleBetween(cachedPlayerPos, cachedFriendlyShipPos);
}