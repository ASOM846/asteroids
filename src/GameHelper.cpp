#include "GameHelper.h"
#include "textureManager.h"
#include "player.h"
#include "drop.h"
#include <cmath>
#include <algorithm>

GameHelper::GameHelper() = default;
GameHelper::~GameHelper() = default;

void GameHelper::checkForFullScreen() {
    if (IsKeyPressed(KEY_F11)) {
        ToggleFullscreen();
    }
}

void GameHelper::setPlayerHealthPtr(int* ptr) {
    playerHealthPtr = ptr;
}

void GameHelper::setTextures(TextureManager& textureManager,
    Player& player) {
    player.setTexture(textureManager.get(TextureId::Player));
}

void GameHelper::setCamera(Camera2D* camera) {
    if (!camera) throw std::runtime_error(std::string("FAILED TO SET CAMERA"));
    if (!player) throw std::runtime_error(std::string("FAILED TO LOAD PLAYER"));

    camera->target = player->getPosition();
    camera->offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    camera->rotation = 0.0f;
    camera->zoom = 1.0f;
}

void GameHelper::drawPosition() {
    if (!player) return;
    Vector2 pos = player->getPosition();
    DrawText(TextFormat("Player Position: (%d, %d)", (int)pos.x, (int)pos.y), 10, 10, 20, WHITE);
}

float GameHelper::getAngleBetween(const Vector2& from, const Vector2& to) const {
    Vector2 delta{ to.x - from.x, to.y - from.y };
    return std::atan2(delta.y, delta.x);
}

float GameHelper::getAngleBetweenPlayerAndFriendlyShip() const {
    if (!player || !ui) return 0.0f;

    Vector2 cachedPlayerPos = player->getPosition();
    Vector2 cachedFriendlyShipPos = customShipManager->getShipPosition(1);

    return getAngleBetween(cachedPlayerPos, cachedFriendlyShipPos);
}