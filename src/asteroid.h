#pragma once
#include <cmath>
#include <numbers>
#include <vector>
#include <algorithm>
#include <cstddef>
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

    // destroyed asteroids counter (incremented when an asteroid is destroyed)
    inline static int sDestroyedCounter = 0;
    static void incrementDestroyed() { ++sDestroyedCounter; }
    static int getDestroyedCount() { return sDestroyedCounter; }
    static void resetDestroyedCount() { sDestroyedCounter = 0; }

    sAsteroid(float px, float py, float dirX, float dirY, float speed, int pRadius,
        const Texture2D* pTexture = nullptr)
        : x(px), y(py), vx(dirX* speed), vy(dirY* speed), active(true),
        health(pRadius), radius(pRadius),
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
            const float diameter = static_cast<float>(radius) * 2.0f;
            Rectangle dst{ x, y, diameter, diameter };
            Vector2 origin{ static_cast<float>(radius), static_cast<float>(radius) };
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

    static const Texture2D* randomTextureForRadius() {
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
        // mark as destroyed (split or fully destroyed)
        incrementDestroyed();
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

            const Texture2D* childTex = randomTextureForRadius();

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
    AsteroidHelper() : asteroidCount(0) {}

    void setAsteroidCount(int count) { asteroidCount = count; }
    int getAsteroidCount() const { return asteroidCount; }
    int getDestroyedAsteroidCount() const { return destroyedCounter; }
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

        // Update destroyed counter from sAsteroid's static counter
        destroyedCounter = sAsteroid::getDestroyedCount();

        if (asteroidCount > 0 && asteroids.size() < static_cast<std::size_t>(asteroidCount))
            generateAsteroid(asteroids, playerPos);
    }

    void renderAsteroids(const std::vector<sAsteroid>& asteroids) const {
        for (const auto& a : asteroids) a.render();
    }

    void generateAsteroid(std::vector<sAsteroid>& asteroids, Vector2 playerPos) {
        const int screenW = GetScreenWidth();
        const int screenH = GetScreenHeight();

        const int radius = GetRandomValue(20, 60);
        const float offscreen = (float)radius + 20.0f;

        const float halfW = screenW / 2.0f;
        const float halfH = screenH / 2.0f;

        const float visibleMinX = playerPos.x - halfW;
        const float visibleMaxX = playerPos.x + halfW;
        const float visibleMinY = playerPos.y - halfH;
        const float visibleMaxY = playerPos.y + halfH;

        auto randFloat = [](float min, float max) {
            if (max <= min) return min;
            constexpr int precision = 1000;
            const float t = (float)GetRandomValue(0, precision) / (float)precision;
            return min + t * (max - min);
        };

        float px = playerPos.x;
        float py = playerPos.y;

        const int edge = GetRandomValue(1, 4);
        switch (edge) {
        case 1: // left of player view
            px = visibleMinX - offscreen;
            py = randFloat(visibleMinY, visibleMaxY);
            break;
        case 2: // top of player view
            px = randFloat(visibleMinX, visibleMaxX);
            py = visibleMinY - offscreen;
            break;
        case 3: // right of player view
            px = visibleMaxX + offscreen;
            py = randFloat(visibleMinY, visibleMaxY);
            break;
        case 4: // bottom of player view
        default:
            px = randFloat(visibleMinX, visibleMaxX);
            py = visibleMaxY + offscreen;
            break;
        }

        const float targetX = randFloat(visibleMinX, visibleMaxX);
        const float targetY = randFloat(visibleMinY, visibleMaxY);

        float dirX = targetX - px;
        float dirY = targetY - py;
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

        float speed = (float)GetRandomValue(2, 5);

        const Texture2D* texPtr = sAsteroid::randomTextureForRadius();

        asteroids.emplace_back(px, py, dirX, dirY, speed, radius, texPtr);
    }

    void generateAsteroidShower(std::vector<sAsteroid>& asteroids, 
        Vector2 playerPos, int count)
    {
        for (int i = 0; i < count; ++i) {
            generateAsteroid(asteroids, playerPos);
        }
    }

    void resetAsteroids(std::vector<sAsteroid>& asteroids) {
        asteroids.clear();
		asteroidCount = 0;
        // Reset instance counter and the static counter used by sAsteroid
        destroyedCounter = 0;
        sAsteroid::resetDestroyedCount();
	}

private:
    int asteroidCount;
    int destroyedCounter = 0;
    const TextureManager* texManager = nullptr;
};