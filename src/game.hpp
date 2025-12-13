#pragma once
#include <cmath>
#include <numbers>
#include <vector>
#include <raylib.h>
#include <functional>

#include "player.h"
#include "laser.h"
#include "asteroid.h"
#include "GameHelper.h"
#include "textureManager.h"
#include "drop.h"
#include "menu.h"
#include "ui.h"
#include "enemy.hpp"
#include "button.hpp"
#include "levelManager.hpp"
#include "customShip.hpp"

enum class eGameState {
    Playing,
    Paused,
    GameOver
};

class Game
{
public:
    Game();
    ~Game();

    void update();
    void render();

    void runLevel(int levelNumber);
private:

    void updatePlaying();
    void renderPlaying();

    void updatePaused();
    void renderPaused();

    void updateGameOver();
    void renderGameOver();

    void initialize();
    
    void setReturnToMenuCallback(std::function<void()> cb) {
        returnToMenuCallback = cb;
    }

    void shutdown();

    void startNewGame();
    void endGame();
    
    void togglePause();

    Player player;
    LaserHelper laserHelper;
    AsteroidHelper asteroidHelper;
    GameHelper gameHelper;
    TextureManager textureManager;
    Ui ui;
    DropHelper dropHelper;
    EnemyManager enemyManager;
    LevelManager levelManager;
    CustomShipManager customShipManager;

    eGameState gameState;

    Camera2D camera;

    std::vector<Laser> lasers;
    std::vector<sAsteroid> asteroids;
    std::vector<sDrop> drops;
    std::vector<sEnemy> enemies;
    std::vector<LevelData> levels;
    std::vector<CustomShip> customShips;

    std::function<void()> returnToMenuCallback;

    friend class WindowManager;
    friend class Menu;
    friend class Ui;
};