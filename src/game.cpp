#include "game.hpp"

/*TO-DO
-add minimap showing player and enemies positions
-implement power-ups that temporarily enhance player abilities
-implement different enemy behaviors and attack patterns and state machines
-implement different game modes like free play, survival with permanent upgrades and upgrade trees, arcade, 

*ideas:
-add levels with upgrades like in vamipre survivors, collecting scrap from destroyed ships and asteroids
    and buying upgrades when the bar of scrap is full
-upgrady w kampani, na przyjk³ad pod koniec mo¿na wybraæ 1/3 któy zostaje na nastêpne poziomy
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
      gameState(eGameState::Playing){
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

    levelManager.updateCurrentLevel();

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
    levelManager.runLevel(levelNumber);
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
        
    saveManager.readData(eDataPosition::LevelsUnlocked);

    levelManager.loadLevelsToMemory();
    levelManager.setUnlockedLevels(saveManager.readData(eDataPosition::LevelsUnlocked));

    levelManager.reset();

    customShipManager.setPointers(&customShips, &textureManager);

    ui.initButtons(GetScreenWidth(), GetScreenHeight());
    ui.setGame(this);


    levelManager.setReturnToMenuCallback([this]()
                                    { this->returnToMenuCallback(); });

    collisionSystem.setPointers(&lasers, &asteroids, &drops,
        &enemies, &customShips, &player, &dropHelper, &gameHelper, &camera);

    camera.initialize(&player);
}

void Game::shutdown() {
    saveManager.writeData(eDataPosition::LevelsUnlocked, levelManager.getUnlockedLevels());
    textureManager.unloadAll();
}

void Game::startNewGame() {
    lasers.clear();
    asteroidHelper.resetAsteroids(asteroids);
    drops.clear();
    player = Player();
    enemyManager.resetEnemies();
    customShipManager.reset();
    gameHelper.setTextures(textureManager, player);
    gameState = eGameState::Playing;
    levelManager.reset();
}

void Game::endGame() {
    // Implementation of logic to end the game
}

void Game::updateGameOver() {
    if (IsKeyPressed(KEY_R)){
        startNewGame();
    }
    else if (IsKeyPressed(KEY_M))   {
        returnToMenuCallback();
    }
}

void Game::renderGameOver() {
    ui.drawStars(player.getPosition());
    DrawText("GAME OVER", GetScreenWidth() / 2 - 190, GetScreenHeight() / 2 - 80, 60, RED);
    DrawText("R - Restart", GetScreenWidth() / 2 - 120, GetScreenHeight() / 2 + 10, 30, GRAY);
    DrawText("M - Menu", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 50, 30, GRAY);
}
    
void Game::togglePause() {
    if (gameState == eGameState::Playing) {
        gameState = eGameState::Paused;
    }
    else if (gameState == eGameState::Paused) {
        gameState = eGameState::Playing;
    }
}