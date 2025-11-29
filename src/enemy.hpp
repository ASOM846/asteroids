#pragma once
#include <cmath>
#include <numbers>
#include <vector>
#include <algorithm>
#include <raylib.h>
#include "textureManager.h"
#include "laser.h"

enum class EnemyType {
    Basic,
    Fast,
    Tank
};

struct sEnemy
{
    Vector2 position;
    bool active;

    EnemyType type;

    int health;
    int radius;

    float speed;
    const Texture2D *texture = nullptr;

    float shootInterval;
    float shootTimer;

    Vector2 getPosition() const { return position; }
    int getRadius() const { return radius; }

    sEnemy(Vector2 pos, EnemyType pType, const Texture2D *tex = nullptr)
        : position(pos), active(true), type(pType), texture(tex)
    {
        struct Params
        {
            int health;
            int radius;
            float speed;
            float shootInterval;
            const Texture2D *tex;
        };

        static constexpr Params table[] = {
            /* Basic */ {30, 20, 2.0f, 1.5f, nullptr},
            /* Fast  */ {40, 15, 4.0f, 0.8f, nullptr},
            /* Tank  */ {100, 30, 2.0f, 2.5f, nullptr}};

        const auto idx = static_cast<size_t>(type);
        const auto &p = table[idx];
        health = p.health;
        radius = p.radius;
        speed = p.speed;
        texture = p.tex;

        // Init shooting timers
        shootInterval = p.shootInterval;
        shootTimer = 0.0f;
    }

    void update(const Vector2 &playerPos, std::vector<Laser>& lasers)
    {
        // odliczaj czas do następnego strzału
        if (shootTimer > 0.0f) {
            shootTimer -= GetFrameTime();
            if (shootTimer < 0.0f) shootTimer = 0.0f;
        }

        // proste AI: podążaj w kierunku gracza
        Vector2 dir = { playerPos.x - position.x, playerPos.y - position.y };
        float len = sqrtf(dir.x*dir.x + dir.y*dir.y);
        if (len > 0.0001f) {
            dir.x /= len; dir.y /= len;
            position.x += dir.x * speed;
            position.y += dir.y * speed;
        }

        shoot(playerPos, lasers);
    }

    void render() const
    {
        if (!active) return;
        if (texture && texture->id != 0) {
            Rectangle src{ 0,0,(float)texture->width,(float)texture->height };
            Rectangle dst{ position.x, position.y, radius * 2.0f, radius * 2.0f };
            Vector2 origin{ static_cast<float>(radius), static_cast<float>(radius) };
            DrawTexturePro(*texture, src, dst, origin, 0.0f, WHITE);
        }
        else {
            DrawCircle((int)position.x, (int)position.y, (float)radius, BLUE);
        }
    }

    void takeDamage(int dmg)
    {
        health -= dmg;
        if (health <= 0) {
            active = false;
        }
    }

    void shoot(Vector2 targetPos, std::vector<Laser>& lasers)
    {
        if (shootTimer > 0.0f) return;

        Vector2 dir = { targetPos.x - position.x, targetPos.y - position.y };
        float len = sqrtf(dir.x*dir.x + dir.y*dir.y);
        if (len > 0.0001f) {
            dir.x /= len; dir.y /= len;

            lasers.emplace_back(position.x, position.y,
                dir.x, dir.y, 6.0f, false);
            shootTimer = shootInterval;
        }
    }
};

class EnemyManager {
public:
    EnemyManager() {}
    ~EnemyManager() {}


    void setPointers(TextureManager* tm, std::vector<sEnemy>* enemyList
        , std::vector<Laser>* laserList)
    {
        texManager = tm;
        enemies = enemyList;
        lasers = laserList;
    }

    void updateEnemies(const Vector2 &playerPos)
    {
        if (!enemies) return;
        for (auto &e : *enemies) {
            if (e.active) e.update(playerPos, *lasers);
            if (!e.active) {
                enemies->erase(std::remove_if(enemies->begin(), enemies->end(),
                    [](const sEnemy &en) { return !en.active; }), enemies->end());
                break;
            }
        }
    }

    void renderEnemies()
    {
        if (!enemies) return;
        for (const auto &e : *enemies) {
            e.render();
        }
    }

    void generateEnemy(EnemyType type)  {
        if (!enemies || !texManager) return;

        const int screenW = GetScreenWidth();
        const int screenH = GetScreenHeight();

        Vector2 pos;
        pos.x = static_cast<float>(GetRandomValue(0, screenW));
        pos.y = static_cast<float>(GetRandomValue(0, screenH));

        sEnemy enemy(pos, type);

        switch (type) {
        case EnemyType::Basic:
            enemy.texture = &texManager->get(TextureId::Enemy1);
            break;
        case EnemyType::Fast:
            enemy.texture = &texManager->get(TextureId::Enemy1);
            break;
        case EnemyType::Tank:
            enemy.texture = &texManager->get(TextureId::Enemy1);
            break;
        }

        enemies->emplace_back(enemy);
    }

private:
    TextureManager* texManager = nullptr;
    std::vector<sEnemy>* enemies = nullptr;
    std::vector<Laser>* lasers = nullptr;
};