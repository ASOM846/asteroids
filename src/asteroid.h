#pragma once
#include <cmath>
#include <numbers>
#include <vector>
#include <algorithm>
#include <raylib.h>
#include "textureManager.h"

struct sAsteroid
{
    float x;
    float y;
    float vx;
    float vy;
    bool active;

    int health;
    int radius;

    float rotation;
    float rotationSpeed;

    const Texture2D* texture;

    inline static const TextureManager* sTexMgr = nullptr;
    static void setTextureManager(const TextureManager* tm) { sTexMgr = tm; }

    sAsteroid(float px, float py, float dirX, float dirY, float speed, int pRadius,
        const Texture2D* pTexture = nullptr)
        : x(px), y(py), vx(dirX* speed), vy(dirY* speed), active(true),
        radius(pRadius), health(pRadius),
        rotation(0.0f),
        rotationSpeed((float)GetRandomValue(-40, 40) / 10.0f),
        texture(pTexture)
    {
    }

    void update(Vector2 playerPos) {
        if (!active)
            return;
        x += vx;
        y += vy;
        float dx = x - playerPos.x;
        float dy = y - playerPos.y;
        const float maxDist = 800.0f;
        if (dx * dx + dy * dy > maxDist * maxDist)
            active = false;
    }

    void render() const {
        if (!active) return;
        if (texture && texture->id != 0) {
            Rectangle src{ 0,0,(float)texture->width,(float)texture->height };
            Rectangle dst{ x, y, radius * 2.0f, radius * 2.0f };
            Vector2 origin{ radius, radius };
            DrawTexturePro(*texture, src, dst, origin, rotation, WHITE);
        }
        else {
            DrawCircle((int)x, (int)y, (float)radius, RED);
        }
    }

    void applyDamage(int amount, std::vector<sAsteroid>& asteroids) {
        if (health > 0)
            health -= amount;
        if (health <= 0)
            splitAsteroid(asteroids);
    }

    static const Texture2D* randomTextureForRadius(int r) {
        if (!sTexMgr) return nullptr;
        int pick = GetRandomValue(0, 7);
        TextureId tid =
            (pick == 0 ? TextureId::MBrownBig1 :
                pick == 1 ? TextureId::MBrownBig2 :
                pick == 2 ? TextureId::MBrownBig3 :
                pick == 3 ? TextureId::MBrownBig4 :
                pick == 4 ? TextureId::MGreyBig1 :
                pick == 5 ? TextureId::MGreyBig2 :
                pick == 6 ? TextureId::MBrownBig3 :
                TextureId::MGreyBig4
                );

        return &sTexMgr->get(tid);
    }

    void splitAsteroid(std::vector<sAsteroid>& asteroids) {
        active = false;
        if (radius <= 20) return;

        const int childRadius = radius / 2;

        float dirX = vx;
        float dirY = vy;
        float len = std::sqrt(dirX * dirX + dirY * dirY);
        if (len > 0.0001f) {
            dirX /= len;
            dirY /= len;
        }
        else {
            const float angle = (float)GetRandomValue(0, 628) / 100.0f;
            dirX = std::cos(angle);
            dirY = std::sin(angle);
        }

        float baseSpeed = std::sqrt(vx * vx + vy * vy);
        if (baseSpeed < 0.5f) baseSpeed = (float)GetRandomValue(2, 5);

        auto spawnChild = [&](float angleDeg) {
            const float rad = angleDeg * (3.14159265358979323846f / 180.0f);
            const float rx = dirX * std::cos(rad) - dirY * std::sin(rad);
            const float ry = dirX * std::sin(rad) + dirY * std::cos(rad);

            const Texture2D* childTex = randomTextureForRadius(childRadius);

            asteroids.emplace_back(x, y, rx, ry, baseSpeed * 1.1f, childRadius, childTex);
            };

        spawnChild(30.0f);
        spawnChild(-30.0f);
    }

    void applyImpulse(float ix, float iy) {
        if (!active) return;
        vx += ix;
        vy += iy;
        const float maxSpeed = 8.0f;
        float s = std::sqrt(vx * vx + vy * vy);
        if (s > maxSpeed && s > 0.0001f) {
            vx = vx / s * maxSpeed;
            vy = vy / s * maxSpeed;
        }
    }

    int getHealth() { return health; }
};

class AsteroidHelper
{
public:
    AsteroidHelper() {}

    void setTextureManager(TextureManager& tm) {
        texManager = &tm;
        sAsteroid::setTextureManager(texManager);
    }

    void updateAsteroids(std::vector<sAsteroid>& asteroids, Vector2 playerPos)
    {
        for (auto& a : asteroids) a.update(playerPos);
        auto it = std::remove_if(asteroids.begin(), asteroids.end(),
            [](const sAsteroid& l) { return !l.active; });
        asteroids.erase(it, asteroids.end());

        if (asteroids.size() < 5)
            generateAsteroid(asteroids);
    }

    void renderAsteroids(const std::vector<sAsteroid>& asteroids) const {
        for (const auto& a : asteroids) a.render();
    }

    void initAsteroids(std::vector<sAsteroid>& asteroids, int count) {
        for (int i = 0; i < count; ++i)
            generateAsteroid(asteroids);
    }

    void generateAsteroid(std::vector<sAsteroid>& asteroids) {
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        int edge = GetRandomValue(1, 4);
        int radius = GetRandomValue(20, 60);
        const int offscreen = radius + 2;
        int px = 0, py = 0;
        float dirX = 0.0f, dirY = 0.0f;

        switch (edge) {
        case 1: px = -offscreen;        py = GetRandomValue(0, screenH); dirX = 1.0f;  dirY = (float)GetRandomValue(-100, 100) / 100.0f; break;
        case 2: px = GetRandomValue(0, screenW); py = -offscreen;        dirX = (float)GetRandomValue(-100, 100) / 100.0f; dirY = 1.0f;  break;
        case 3: px = screenW + offscreen; py = GetRandomValue(0, screenH); dirX = -1.0f; dirY = (float)GetRandomValue(-100, 100) / 100.0f; break;
        case 4: px = GetRandomValue(0, screenW); py = screenH + offscreen; dirX = (float)GetRandomValue(-100, 100) / 100.0f; dirY = -1.0f; break;
        }

        float len = std::sqrt(dirX * dirX + dirY * dirY);
        if (len > 0.0001f) {
            dirX /= len;
            dirY /= len;
        }

        float speed = (float)GetRandomValue(2, 5);

        const Texture2D* texPtr = sAsteroid::randomTextureForRadius(radius);

        asteroids.emplace_back((float)px, (float)py, dirX, dirY, speed, radius, texPtr);
    }

private:
    const TextureManager* texManager = nullptr;
};