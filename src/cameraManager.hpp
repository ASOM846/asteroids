#pragma once
#include <raylib.h>
#include <stdexcept>
#include <algorithm>
#include "player.h"

struct ScreenShake
{
    Vector2 offset{0.0f, 0.0f};
    float intensity = 0.0f;
    float totalTime = 0.0f;
    float timeLeft = 0.0f;

    void trigger(float inten, float duration)
    {
        intensity = std::max(intensity, inten);
        totalTime = std::max(totalTime, duration);
        timeLeft = std::max(timeLeft, duration);
    }

    void update(float dt)
    {
        if (timeLeft <= 0.0f)
        {
            offset = {0.0f, 0.0f};
            intensity = 0.0f;
            return;
        }
        timeLeft -= dt;
        float t = (totalTime > 0.0f) ? (timeLeft / totalTime) : 0.0f;
        float cur = intensity * t;
        float rx = (static_cast<float>(GetRandomValue(-100, 100)) / 100.0f) * cur;
        float ry = (static_cast<float>(GetRandomValue(-100, 100)) / 100.0f) * cur;
        offset = {rx, ry};
        if (timeLeft <= 0.0f)
            offset = {0.0f, 0.0f};
    }
};

class CameraManager {
public:
    CameraManager() : player(nullptr) {}

    ~CameraManager() = default;

    void initialize(Player* pPlayer) {
        player = pPlayer;

        if(player == nullptr)
            throw std::runtime_error("CameraManager: Player pointer is null");

        camera.target = player->getPosition();
        camera.offset = Vector2{GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
    }

    void updateCamera() {
        if (player == nullptr)
            return;

        screenShake.update(GetFrameTime());
        Vector2 p = player->getPosition();
        camera.target = Vector2{p.x + screenShake.offset.x, p.y + screenShake.offset.y};
    }

    void triggerShake(float intensity, float duration) {
        screenShake.trigger(intensity, duration);
    }

    void triggerShake()
    {
        screenShake.trigger(5.0f, 0.3f);
    }

    Camera2D& getCamera() { return camera; }

private:
    Camera2D camera;
    ScreenShake screenShake;

    Player* player;

};
