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

void GameHelper::handleCollision(std::vector<Laser>& lasers,
    std::vector<sAsteroid>& asteroids,
    const Rectangle& playerRect) {
    int damageToPlayer = 0;

    for (auto& a : asteroids) {
        if (!a.active) continue;

        //kolizja asteroidy z laserami
        for (auto& l : lasers) {
            if (!l.active) continue;

            if (CheckCollisionCircles(Vector2{ l.x, l.y }, (float)l.radius,
                Vector2{ a.x, a.y }, (float)a.radius)) {

                a.applyDamage(l.getDamage(), asteroids);

                float lvx = l.vx;
                float lvy = l.vy;
                float len = std::sqrt(lvx * lvx + lvy * lvy);
                if (len > 0.0001f) {
                    lvx /= len;
                    lvy /= len;
                }
                
                float impulse = (float)l.getDamage() / (float)a.radius * 2.0f;
                a.applyImpulse(lvx * impulse, lvy * impulse);
                l.active = false;

                if (!a.active) {
                    maybeSpawnDrop(a.x, a.y);
                    player->increaseScore(a.radius);
                }
                break;
            }
        }

        //kolizja asteroidy z graczem
        if (CheckCollisionCircleRec(Vector2{ a.x, a.y },
            (float)a.radius, playerRect)) {
            damageToPlayer += a.radius;
            a.active = false;
        }
    }

    if (damageToPlayer > 0 && playerHealthPtr)
        player->takeDamage(damageToPlayer);
}

void GameHelper::checkCollisionPlayerDrop(std::vector<sDrop>& drops, Rectangle playerRect) {
    for (auto& d : drops) {
        if (!d.active) continue;
        if (CheckCollisionRecs(playerRect, d.getRect())) {
            if (d.getType() == DropType::Health && playerHealthPtr) {
                *playerHealthPtr = std::min(100, *playerHealthPtr + 20);
            }
            if (d.getType() == DropType::Shield && playerShieldPtr) {
                *playerShieldPtr = std::min(100, *playerShieldPtr + 20);
            }
            if(d.getType() == DropType::Ammo)   {
                player->increaseAmmo(50);
            }

            d.active = false;
        }
    }
}


void GameHelper::checkCollisionLaserPlayerEnemy()
{
    if (!enemies || !lasers || !player) return;

    for (auto& e : *enemies) {
        if (!e.active) continue;

        for (auto& l : *lasers) {
            if (!l.active) continue;

            if (l.getIsPlayerOwned()) {
                // player's laser -> check collision with enemy
                if (CheckCollisionCircles(Vector2{ l.x, l.y }, (float)l.radius,
                    e.getPosition(), (float)e.getRadius())) {
                    e.takeDamage(l.getDamage());
                    l.active = false;
                    break;
                }
            }
            else {
                // enemy laser -> check collision with player
                if (CheckCollisionCircleRec(Vector2{ l.x, l.y }, (float)l.radius,
                    player->getRect())) {
                    player->takeDamage(l.getDamage());
                    l.active = false;
                    break;
                }
            }
        }
    }
}

void GameHelper::maybeSpawnDrop(float x, float y) {
    if (!dropHelper || !drops) return;
    if (GetRandomValue(0, 99) < (int)(dropSpawnChance * 100.0f)) {
        dropHelper->spawnRandomDrop(*drops, x, y);
    }
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