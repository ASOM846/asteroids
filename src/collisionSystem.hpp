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
#include "drop.h"
#include "game.hpp"
#include "cameraManager.hpp"

class Player;

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
                     CameraManager* pCameraManager) {
        lasers = pLasers;
        asteroids = pAsteroids;
        drops = pDrops;
        enemies = pEnemies;
        customShips = pCustomShips;
        player = pPlayer;
        dropHelper = pDropHelper;
        gameHelper = pGameHelper;
        cameraManager = pCameraManager;
    }

    void handleCollision() {
        if(!lasers || !asteroids || !player 
            || !drops || !enemies || !customShips) return;

        Rectangle playerRect = player->getRect();

        for (auto& a : *asteroids) {
            if (!a.active) continue;
            for (auto& l : *lasers) {
                if (!l.active) continue;
                if (CheckCollisionCircles(Vector2{ l.x, l.y }, (float)l.radius,
                                          Vector2{ a.x, a.y }, (float)a.radius)) {
                    a.applyDamage(l.getDamage(), *asteroids);
                    float lvx = l.vx, lvy = l.vy;
                    float len = std::sqrt(lvx * lvx + lvy * lvy);
                    if (len > 0.0001f) { lvx /= len; lvy /= len; }
                    float impulse = (float)l.getDamage() / (float)a.radius * 2.0f;
                    a.applyImpulse(lvx * impulse, lvy * impulse);
                    l.active = false;
                    if (!a.active) {
                        dropHelper->maybeSpawnDrop(a.x, a.y);
                        player->increaseScore(a.radius);
                    }
                    break;
                }
            }
            if (CheckCollisionCircleRec(Vector2{ a.x, a.y }, (float)a.radius, playerRect)) {
                player->takeDamage(a.radius);
                cameraManager->triggerShake();
                a.active = false;
            }
        }

        for (auto& d : *drops) {
            if (!d.active) continue;
            if (CheckCollisionRecs(playerRect, d.getRect())) {
                if (d.getType() == DropType::Health) player->heal(20);
                if (d.getType() == DropType::Shield) player->healShield(20);
                if (d.getType() == DropType::Ammo) player->increaseAmmo(50);
                d.active = false;
            }
        }

        // Handle laser collisions in a single pass so checks run regardless of enemy count
        for (auto& l : *lasers) {
            if (!l.active) continue;

            if (l.getIsPlayerOwned()) {
                // Player lasers: check enemies first
                for (auto& e : *enemies) {
                    if (!e.active) continue;
                    if (CheckCollisionCircles(Vector2{ l.x, l.y }, (float)l.radius,
                        e.getPosition(), (float)e.getRadius())) {
                        e.takeDamage(l.getDamage());
                        l.active = false;
                        break;
                    }
                }
                if (!l.active) continue;

                // Then check friendly/custom ships
                if (customShips) {
                    for (size_t i = 0; i < customShips->size(); ++i) {
                        auto& cs = (*customShips)[i];
                        if (cs.texture.id == 0) continue;
                        if (CheckCollisionCircleRec(Vector2{ l.x, l.y }, (float)l.radius,
                            cs.getRect())) {
                            cs.health -= l.getDamage();
                            l.active = false;
                            // remove ship if destroyed
                            if (cs.health <= 0) {
                                customShips->erase(customShips->begin() + i);
                                --i;
                            }
                            break;
                        }
                    }
                }
            }
            else {
                // Enemy/projectile not owned by player -> can hit player
                if (CheckCollisionCircleRec(Vector2{ l.x, l.y }, (float)l.radius,
                    playerRect)) {
                    player->takeDamage(l.getDamage());
                    l.active = false;
                }
            }
        }
    }



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