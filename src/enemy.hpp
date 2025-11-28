#pragma once
#include <cmath>
#include <numbers>
#include <vector>
#include <algorithm>
#include <raylib.h>
#include "textureManager.h"

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

    sEnemy(Vector2 pos, EnemyType pType, const Texture2D *tex = nullptr)
        : position(pos), active(true), type(pType), texture(tex)
    {
        struct Params
        {
            int health;
            int radius;
            float speed;
            const Texture2D *tex;
        };

        static constexpr Params table[] = {
            /* Basic */ {3, 20, 2.0f, nullptr},
            /* Fast  */ {2, 15, 4.0f, nullptr},
            /* Tank  */ {5, 30, 1.0f, nullptr}};

        const auto idx = static_cast<size_t>(type);
        const auto &p = table[idx];
        health = p.health;
        radius = p.radius;
        speed = p.speed;
        texture = p.tex;
    }

    void update(const Vector2 &playerPos)
    {
        // proste AI: podążaj w kierunku gracza
        Vector2 dir = { playerPos.x - position.x, playerPos.y - position.y };
        float len = sqrtf(dir.x*dir.x + dir.y*dir.y);
        if (len > 0.0001f) {
            dir.x /= len; dir.y /= len;
            position.x += dir.x * speed;
            position.y += dir.y * speed;
        }
    }

    void render() const
    {
        if (!active) return;
        if (texture && texture->id != 0) {
            Rectangle src{ 0,0,(float)texture->width,(float)texture->height };
            Rectangle dst{ position.x, position.y, radius * 2.0f, radius * 2.0f };
            Vector2 origin{ radius, radius };
            DrawTexturePro(*texture, src, dst, origin, 0.0f, WHITE);
        }
        else {
            DrawCircle((int)position.x, (int)position.y, (float)radius, BLUE);
        }
    }
};

class EnemyManager {
public:
    EnemyManager() {}
    ~EnemyManager() {}


    void setPointers(TextureManager* tm, std::vector<sEnemy>* enemyList)
    {
        texManager = tm;
        enemies = enemyList;
    }

    void updateEnemies(const Vector2 &playerPos)
    {
        if (!enemies) return;
        for (auto &e : *enemies) {
            if (e.active) e.update(playerPos);
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

        // Przypisz teksturę na podstawie typu wroga
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
    
};