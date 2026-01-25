#include "game.hpp"

/*TO-DO
-add minimap showing player and enemies positions
-implement power-ups that temporarily enhance player abilities
-implement different enemy behaviors and attack patterns and state machines
-implement different game modes like free play, survival with permanent upgrades and upgrade trees, arcade, 

*ideas:
-add levels with upgrades like in vamipre survivors, collecting scrap from destroyed ships and asteroids
    and buying upgrades when the bar of scrap is full
-upgrady w kampani, na przyjk�ad pod koniec mo�na wybra� 1/3 kt�y zostaje na nast�pne poziomy
*/

Game::Game()
    : player(),
      laserHelper(),
      asteroidHelper(),
      gameHelper(),
      textureManager(),
      ui(),
      dropHelper(),
      enemyManager(),
      levelManager(),
      customShipManager(),
      collisionSystem(),
      camera(),
      saveManager(),
      upgradeSystem(),
      gameState(eGameState::Playing),
      currency(0),
      isArcadeMode(false),
      arcadeTimer(0.0f),
      arcadeWave(1),
      waveSpawnTimer(0.0f){
}

Game::~Game() {
    shutdown();
}

void Game::update() {
    switch (gameState) {
    case eGameState::Playing:
        updatePlaying();
        break;
    case eGameState::Paused:
        updatePaused();
        break;
    case eGameState::GameOver:
        updateGameOver();
        break;
    }
}

void Game::render() {
    switch (gameState) {
    case eGameState::Playing:
        renderPlaying();
        break;
    case eGameState::Paused:
        renderPaused();
        break;
    case eGameState::GameOver:
        renderGameOver();
        break;
    }
}

void Game::updatePlaying() {
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P))
    {
        togglePause();
        return;
    }

    if(IsKeyPressed(KEY_L))
        std::cout <<  levelManager.getUnlockedLevels() << std::endl;
    collisionSystem.handleCollision();
    player.update();
    camera.updateCamera();
    laserHelper.updateLasers(lasers, (int)player.getPosition().x, (int)player.getPosition().y);
    asteroidHelper.updateAsteroids(asteroids, player.getPosition());
    enemyManager.updateEnemies(player.getPosition());
    customShipManager.updateShips();

    if (isArcadeMode) {
        updateArcadeMode();
    } else {
        levelManager.updateCurrentLevel();
    }

    dropHelper.updateDrops(drops);

    player.tryShoot(lasers);
}

void Game::renderPlaying() {
    const Vector2 playerWorldPos = player.getPosition();
    ui.drawStars(playerWorldPos);
    BeginMode2D(camera.getCamera());

    laserHelper.renderLasers(lasers);
    asteroidHelper.renderAsteroids(asteroids);
    dropHelper.renderDrops(drops);
    enemyManager.renderEnemies();
    customShipManager.renderShips();
    player.render();

    EndMode2D();

    const Vector2* friendlyShipPos = customShips.empty() ? nullptr : &customShips.front().position;

    ui.draw(player.getHealth(), player.getShield(),
            player.getAmmo(), player.getMaxAmmo(),
            player.getScore(), levelManager.getRemainingLevelTime(),
            playerWorldPos, friendlyShipPos,
            levelManager.getCurrentLevelData(),
            levelManager.getProgressAccumulator());

    // Display arcade mode info
    if (isArcadeMode) {
        int screenWidth = GetScreenWidth();
        std::string waveText = "WAVE: " + std::to_string(arcadeWave);
        DrawText(waveText.c_str(), screenWidth / 2 - 60, 50, 30, GOLD);
        
        std::string currencyText = "Credits: " + std::to_string(currency);
        DrawText(currencyText.c_str(), screenWidth - 200, 20, 24, SKYBLUE);
    }
    
    // Display combo counter
    if (player.getComboCount() > 1) {
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        
        std::string comboText = std::to_string(player.getComboCount()) + "x COMBO!";
        float multiplier = player.getComboMultiplier();
        std::string multText = "Score x" + std::to_string(multiplier).substr(0, 4);
        
        Color comboColor = YELLOW;
        if (player.getComboCount() >= 10) comboColor = RED;
        else if (player.getComboCount() >= 5) comboColor = ORANGE;
        
        DrawText(comboText.c_str(), screenWidth - 250, screenHeight / 2 - 50, 36, comboColor);
        DrawText(multText.c_str(), screenWidth - 250, screenHeight / 2 - 10, 24, SKYBLUE);
    }

    levelManager.drawLevelEndOverlay(GetScreenWidth(), GetScreenHeight());
}

void Game::updatePaused() {
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P))
    {
        togglePause();
    }
    ui.updatePauseOverlay(GetScreenWidth(), GetScreenHeight());
}

void Game::renderPaused() {
    const Vector2 playerWorldPos = player.getPosition();
    ui.drawStars(playerWorldPos);
    BeginMode2D(camera.getCamera());

    laserHelper.renderLasers(lasers);
    asteroidHelper.renderAsteroids(asteroids);
    dropHelper.renderDrops(drops);
    enemyManager.renderEnemies();
    customShipManager.renderShips();
    player.render();

    EndMode2D();

    const Vector2* friendlyShipPos = customShips.empty() ? nullptr : &customShips.front().position;

    ui.draw(player.getHealth(), player.getShield(),
            player.getAmmo(), player.getMaxAmmo(),
            player.getScore(), levelManager.getRemainingLevelTime(),
            playerWorldPos, friendlyShipPos, 
            levelManager.getCurrentLevelData(),
            levelManager.getProgressAccumulator());

    ui.renderPauseOverlay(GetScreenWidth(), GetScreenHeight());
}

void Game::runLevel(int levelNumber) {
    startNewGame();
    isArcadeMode = false;
    levelManager.runLevel(levelNumber);
}

void Game::startArcadeMode() {
    startNewGame();
    isArcadeMode = true;
    // Arcade mode doesn't use level manager, instead we'll use continuous spawning
}

void Game::startQuickStart() {
    startNewGame();
    isArcadeMode = false;
    // Quick Start - free play mode with no objectives
    // Just spawn some asteroids for practice
    for (int i = 0; i < 8; ++i) {
        asteroidHelper.generateAsteroid(asteroids, player.getPosition());
    }
}

void Game::initialize() {
    textureManager.loadAll();

    gameHelper.setPlayer(&player);
    
    gameHelper.setTextures(textureManager, player);
    gameHelper.setCustomShipManager(&customShipManager);

    enemyManager.setPointers(&textureManager, &enemies, &lasers, &player);

    asteroidHelper.setTextureManager(textureManager);

    dropHelper.setTextureManager(textureManager);
    dropHelper.setDrops(&drops);
    
    levelManager.setPointers(&levels, &dropHelper, &player,
        &asteroidHelper, &customShipManager, 
        &enemyManager, &ui, &drops, &enemies);
        
    // Load save data
    currency = saveManager.readData(eDataPosition::Currency);
    std::string upgradeData = saveManager.readStringData(eDataPosition::UpgradeData);
    upgradeSystem.loadFromString(upgradeData);

    levelManager.loadLevelsToMemory();
    levelManager.setUnlockedLevels(saveManager.readData(eDataPosition::LevelsUnlocked));

    levelManager.reset();

    customShipManager.setPointers(&customShips, &textureManager);

    ui.initButtons(GetScreenWidth(), GetScreenHeight());
    ui.setGame(this);


    levelManager.setReturnToMenuCallback([this]()
                                    { this->returnToMenuCallback(); });

    collisionSystem.setPointers(&lasers, &asteroids, &drops,
        &enemies, &customShips, &player, &dropHelper, &gameHelper, &camera, this);

    camera.initialize(&player);
}

void Game::shutdown() {
    saveManager.writeData(eDataPosition::LevelsUnlocked, levelManager.getUnlockedLevels());
    saveManager.writeData(eDataPosition::Currency, currency);
    
    std::string upgradeData;
    upgradeSystem.saveToString(upgradeData);
    saveManager.writeStringData(eDataPosition::UpgradeData, upgradeData);
    
    textureManager.unloadAll();
}

void Game::startNewGame() {
    lasers.clear();
    asteroidHelper.resetAsteroids(asteroids);
    drops.clear();
    player = Player();
    
    // Apply upgrade bonuses and reset stats to full
    player.resetWithUpgrades(upgradeSystem);
    
    enemyManager.resetEnemies();
    customShipManager.reset();
    gameHelper.setTextures(textureManager, player);
    gameState = eGameState::Playing;
    levelManager.reset();
    ui.resetAll();
    
    // Reset arcade mode variables
    arcadeTimer = 0.0f;
    arcadeWave = 1;
    waveSpawnTimer = 0.0f;
}

void Game::endGame() {
    // Implementation of logic to end the game
}

void Game::updateGameOver() {
    if (IsKeyPressed(KEY_R)){
        startNewGame();
        if (isArcadeMode) {
            // Reset arcade mode
            arcadeTimer = 0.0f;
            arcadeWave = 1;
            waveSpawnTimer = 0.0f;
        }
    }
    else if (IsKeyPressed(KEY_M))   {
        returnToMenuCallback();
    }
}

void Game::renderGameOver() {
    ui.drawStars(player.getPosition());
    DrawText("GAME OVER", GetScreenWidth() / 2 - 190, GetScreenHeight() / 2 - 120, 60, RED);
    
    // Display final stats
    int score = player.getScore();
    std::string scoreText = "Final Score: " + std::to_string(score);
    DrawText(scoreText.c_str(), GetScreenWidth() / 2 - 150, GetScreenHeight() / 2 - 40, 30, WHITE);
    
    if (isArcadeMode) {
        std::string waveText = "Survived to Wave " + std::to_string(arcadeWave);
        DrawText(waveText.c_str(), GetScreenWidth() / 2 - 180, GetScreenHeight() / 2, 26, SKYBLUE);
        
        int highScore = saveManager.readData(eDataPosition::HighScore);
        if (score > highScore) {
            saveManager.writeData(eDataPosition::HighScore, score);
            DrawText("NEW HIGH SCORE!", GetScreenWidth() / 2 - 160, GetScreenHeight() / 2 + 35, 28, GOLD);
        } else {
            std::string highScoreText = "High Score: " + std::to_string(highScore);
            DrawText(highScoreText.c_str(), GetScreenWidth() / 2 - 140, GetScreenHeight() / 2 + 35, 24, GRAY);
        }
    }
    
    DrawText("R - Restart", GetScreenWidth() / 2 - 120, GetScreenHeight() / 2 + 80, 30, GRAY);
    DrawText("M - Menu", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 120, 30, GRAY);
}
    
void Game::togglePause() {
    if (gameState == eGameState::Playing) {
        gameState = eGameState::Paused;
    }
    else if (gameState == eGameState::Paused) {
        gameState = eGameState::Playing;
    }
}

void Game::updateArcadeMode() {
    arcadeTimer += GetFrameTime();
    waveSpawnTimer += GetFrameTime();
    
    // Progressive difficulty - increase wave based on constant
    int currentWave = (int)(arcadeTimer / WAVE_DURATION_SECONDS) + 1;
    if (currentWave > arcadeWave) {
        arcadeWave = currentWave;
    }
    
    // Spawn asteroids periodically
    int desiredAsteroids = BASE_ASTEROIDS_PER_WAVE + arcadeWave * ASTEROIDS_INCREMENT_PER_WAVE;
    if (asteroids.size() < (size_t)desiredAsteroids && waveSpawnTimer > ASTEROID_SPAWN_INTERVAL) {
        asteroidHelper.generateAsteroid(asteroids, player.getPosition());
        waveSpawnTimer = 0.0f;
    }
    
    // Spawn enemies based on wave
    if (arcadeWave >= 2) {
        int basicEnemies = arcadeWave - 1;
        int fastEnemies = arcadeWave >= 4 ? (arcadeWave - 3) : 0;
        int tankEnemies = arcadeWave >= 6 ? (arcadeWave - 5) / 2 : 0;
        
        enemyManager.setDesiredCount(EnemyType::Basic, basicEnemies);
        enemyManager.setDesiredCount(EnemyType::Fast, fastEnemies);
        enemyManager.setDesiredCount(EnemyType::Tank, tankEnemies);
    }
}