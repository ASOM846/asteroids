#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include <random>

class Ui {
public:
    Ui();
    ~Ui();

    void draw(int health, int shield, 
            int ammo, int maxAmmo, int score,
            float remainingLevelTime);

    void drawStars(Vector2 playerPos);

private:
    struct Star {
        Vector2 position;   // screen-space position
        float size;
        float twinkleSpeed;
        float twinkleOffset;
    };

    struct StarLayer {
        std::vector<Star> stars;
        float parallax;
        Color color;
    };

    std::vector<StarLayer> starLayers;
    std::mt19937 rng;
    int cachedWidth;
    int cachedHeight;
    bool hasLastPlayerPos;
    Vector2 lastPlayerPos;

    void initStarLayers(int screenW, int screenH);
};