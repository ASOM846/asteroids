#include "game.h"

Game::Game() : screenWidth(1280), screenHeight(720), player(),
BcgColour(BLACK), gameHelper(), camera(),
gameState(GameState::Menu),
menu(this) {
    initialize();
    runLoop();
    shutdown();
}

Game::~Game() {}

void Game::setGameState(GameState newState) {
    if (gameState == newState) return;
    gameState = newState;
    if (gameState == GameState::Playing) {
        startGame();
    }
    else if (gameState == GameState::GameOver) {
        endGame();
    }
}

void Game::initialize() {
    InitWindow(screenWidth, screenHeight, "Asteroids Game");
    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    camera.target = player.getPosition();
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    gameHelper.setPlayer(&player);
    gameHelper.setPlayerHealthPtr(player.getHealthPtr());
    gameHelper.setPlayerShieldPtr(player.getShieldPtr());
    textureManager.loadAll();
    gameHelper.setTextures(textureManager, player);
    asteroidHelper.setTextureManager(textureManager);
    dropHelper.setTextureManager(textureManager);
    gameHelper.setDropHelper(&dropHelper);
    gameHelper.setDrops(&drops);
    gameHelper.setDropChance(1.00f);
}

void Game::shutdown() {
    textureManager.unloadAll();
    CloseWindow();
}

void Game::runLoop() {
    while (!WindowShouldClose()) {
        update();
        render();
    }
}

void Game::startGame() {
    lasers.clear();
    asteroids.clear();
    drops.clear();
    player = Player();
    gameHelper.setPlayerHealthPtr(player.getHealthPtr());
    gameHelper.setTextures(textureManager, player);
    asteroidHelper.initAsteroids(asteroids, 5);
    gameState = GameState::Playing;
}

void Game::endGame() {
    gameState = GameState::GameOver;
}

void Game::togglePause() {
    if (gameState == GameState::Playing) gameState = GameState::Paused;
    else if (gameState == GameState::Paused) gameState = GameState::Playing;
}

void Game::update() {
    gameHelper.checkForFullScreen();

    switch (gameState) {
    case GameState::Menu:     updateMenu(); break;
    case GameState::Playing:  updatePlaying(); break;
    case GameState::Paused:   updatePaused(); break;
    case GameState::GameOver: updateGameOver(); break;
    }
}

void Game::render() {
    BeginDrawing();
    ClearBackground(BcgColour);

    switch (gameState) {
    case GameState::Menu:     renderMenu(); break;
    case GameState::Playing:  renderPlaying(); break;
    case GameState::Paused:   renderPaused(); break;
    case GameState::GameOver: renderGameOver(); break;
    }

    EndDrawing();
}

void Game::updateMenu() {
    menu.update();
    if (gameState == GameState::Playing) {
        startGame();
    }
}

void Game::renderMenu() {
    menu.render();
}

void Game::updatePlaying() {
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) {
        togglePause();
        return;
    }

    player.update();
    camera.target = player.getPosition();
    laserHelper.updateLasers(lasers, (int)player.getPosition().x, (int)player.getPosition().y);
    asteroidHelper.updateAsteroids(asteroids, player.getPosition());

    gameHelper.handleCollision(lasers, asteroids, player.getRect());
    gameHelper.checkCollisionPlayerDrop(drops, player.getRect());

    dropHelper.updateDrops(drops);

    player.tryShoot(lasers);
}

void Game::renderPlaying() {
    BeginMode2D(camera);

    laserHelper.renderLasers(lasers);
    asteroidHelper.renderAsteroids(asteroids);
    dropHelper.renderDrops(drops);
    player.render();

    EndMode2D();

    ui.draw(player.getHealth(), player.getShield(), 
            player.getAmmo(), player.getMaxAmmo(),
        player.getScore());

    gameHelper.drawPosition();
}

void Game::updatePaused() {
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) {
        togglePause();
    }
}

void Game::renderPaused() {
    BeginMode2D(camera);

    laserHelper.renderLasers(lasers);
    asteroidHelper.renderAsteroids(asteroids);
    dropHelper.renderDrops(drops);
    player.render();

    EndMode2D();

    ui.draw(player.getHealth(), player.getShield(), 
            player.getAmmo(),player.getMaxAmmo(),
            player.getScore());

    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    DrawText("PAUZA", screenWidth / 2 - 90, screenHeight / 2 - 40, 50, WHITE);
    DrawText("ESC/P - Wznow", screenWidth / 2 - 150, screenHeight / 2 + 30, 30, GRAY);
}

void Game::updateGameOver() {
    if (IsKeyPressed(KEY_R)) {
        startGame();
    }
    else if (IsKeyPressed(KEY_M)) {
        gameState = GameState::Menu;
    }
}

void Game::renderGameOver() {
    DrawText("GAME OVER", screenWidth / 2 - 190, screenHeight / 2 - 80, 60, RED);
    DrawText("R - Restart", screenWidth / 2 - 120, screenHeight / 2 + 10, 30, GRAY);
    DrawText("M - Menu", screenWidth / 2 - 100, screenHeight / 2 + 50, 30, GRAY);
}