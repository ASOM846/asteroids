#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include <random>

#include "button.hpp"

class Game;

class Ui {
public:
    Ui();
    ~Ui();

    void initButtons(int sW, int sH);
    void setGame(Game* g);

	void draw(int health, int shield,
		int ammo, int maxAmmo, int score,
		float remainingLevelTime,
		Vector2 playerWorldPos,
		const Vector2* friendlyShipPos = nullptr);

    void drawArrowAngled(float angle);

    void drawStars(Vector2 playerPos);

    void renderPauseOverlay(int sW, int sH);
    void updatePauseOverlay(int sW, int sH);

private:
    struct Star {
        Vector2 position;
        float size;
        float twinkleSpeed;
        float twinkleOffset;
    };

    struct StarLayer {
        std::vector<Star> stars;
        float parallax;
        Color color;
    };

    Game* game;

    std::vector<StarLayer> starLayers;
    std::mt19937 rng;
    int cachedWidth;
    int cachedHeight;
    bool hasLastPlayerPos;
    Vector2 lastPlayerPos;

    //paused state overlay buttons
    Button resumeButton;
    Button mainMenuButton;


    void initStarLayers(int screenW, int screenH);
};