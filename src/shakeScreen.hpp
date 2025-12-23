#pragma once
#include <raylib.h>
#include <raymath.h>

struct ScreenShake
{
    Vector2 offset{0.0f, 0.0f};
    float intensity = 0.0f; // starting intensity (pixels)
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