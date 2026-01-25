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
#include "collisionSystem.hpp"
#include "cameraManager.hpp"
#include "saveManager.hpp"
#include "upgradeSystem.hpp"

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
    void startArcadeMode();
    
    void awardCurrency(int amount) { currency += amount; }
    
    int getCurrency() const { return currency; }
    UpgradeSystem& getUpgradeSystem() { return upgradeSystem; }
private:

    void updatePlaying();
    void renderPlaying();
    
    void updateArcadeMode();

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
    CollisionSystem collisionSystem;
    CameraManager camera;
    SaveManager saveManager;
    UpgradeSystem upgradeSystem;

    eGameState gameState;
    
    int currency;
    bool isArcadeMode;
    float arcadeTimer;
    int arcadeWave;
    float waveSpawnTimer;

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