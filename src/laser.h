#pragma once
#include <raylib.h>
#include <vector>
#include <cmath>
#include <algorithm>

struct Laser
{
    float x;
    float y;
    float vx;
    float vy;
    bool active;
    int radius;
    int damage;

    Laser(float px, float py, float dirX, float dirY, float speed)
        : x(px), y(py), vx(dirX* speed), vy(dirY* speed), active(true), 
            radius(3), damage(10) {
    }

    void update(int screenW, int screenH)
    {
        if (!active) return;
        x += vx;
        y += vy;
        if (x < 0 || x > screenW || y < 0 || y > screenH) active = false;
    }

    void render() const
    {
        if (!active) return;
        DrawCircle((int)x, (int)y, 3.0f, RED);
    }

    int getDamage() { return damage; }
};

class LaserHelper
{
public:
    LaserHelper() = default;
    ~LaserHelper() = default;

    void updateLasers(std::vector<Laser>& lasers, int screenW, int screenH)
    {
        for (auto& l : lasers) l.update(screenW, screenH);
        auto it = std::remove_if(lasers.begin(), lasers.end(),
            [](const Laser& l) { return !l.active; });
        lasers.erase(it, lasers.end());
    }

    void renderLasers(const std::vector<Laser>& lasers) const
    {
        for (const auto& l : lasers) l.render();
    }
};