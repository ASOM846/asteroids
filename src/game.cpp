#include "game.h"

Game::Game()
    : screenWidth(1280),
    screenHeight(720),
    player(),
    laserHelper(),
    asteroidHelper(),
    gameHelper(),
    textureManager(),
    ui(),
    menu(this, &ui, &levels),
    dropHelper(),
    enemyManager(),
    BcgColour(BLACK),
    gameState(GameState::Menu),
    camera{},
    lasers(),
    asteroids(),
    drops(),
    enemies() {
    initialize();
    runLoop();
    shutdown();
}

Game::~Game() {}

void Game::setGameState(GameState newState) {
    if (gameState == newState) return;
    gameState = newState;
    if (gameState == GameState::Menu) {
        menu.setMenuState(MenuState::Main);
    }
    if (gameState == GameState::Playing) {
        startGame();
    }
    else if (gameState == GameState::GameOver) {
        endGame();
    }
}

void Game::runLevel(int levelNumber) {
    levelManager.runLevel(levelNumber);
}

void Game::initialize() {
    InitWindow(screenWidth, screenHeight, "Asteroids Game");
    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    textureManager.loadAll();

    gameHelper.setPlayer(&player);
    gameHelper.setPlayerHealthPtr(player.getHealthPtr());
    gameHelper.setPlayerShieldPtr(player.getShieldPtr());
    gameHelper.setTextures(textureManager, player);
    gameHelper.setDropHelper(&dropHelper);
    gameHelper.setDrops(&drops);
    gameHelper.setDropChance(1.00f);
    gameHelper.setPointers(&enemies, &lasers);
    gameHelper.setCamera(&camera);

    enemyManager.setPointers(&textureManager, &enemies, &lasers);

    asteroidHelper.setTextureManager(textureManager);

    dropHelper.setTextureManager(textureManager);

    levelManager.setPointers(&levels, &dropHelper, &player,
        &asteroidHelper, &drops, &enemies);
    levelManager.loadLevelsToMemory();
    levelManager.reset();

    ui.initButtons(screenWidth, screenHeight);
    ui.setGame(this);

    // powiadom Game, aby po zakończeniu poziomu wrócił do menu
    levelManager.setOnLevelComplete([this]() {
        this->setGameState(GameState::Menu);
        });
}

void Game::shutdown() {
    // Najpierw rozładuj zasoby zarządzane przez aplikację (unikamy zależności od wewnętrznych destruktorów raylib)
    textureManager.unloadAll();

    // UWAGA: W wykorzystywanej wersji raylib istnieje błąd w obsłudze default font:
    // UnloadFontDefault() zwalnia obrazy glyphów, które wskazują na wcześniej zwolnioną pamięć,
    // co powoduje naruszenie pamięci podczas CloseWindow().
    // Aby tego uniknąć bez modyfikacji raylib, pomijamy wywołanie CloseWindow() i pozwalamy
    // procesowi natywnemu zakończyć kontekst graficzny razem z procesem aplikacji.
    // Jeśli zaktualizujesz raylib do wersji poprawiającej ten bug, przywróć wywołanie CloseWindow().
    //
    // CloseWindow(); // wyłączone z powodu błędu w tej wersji raylib
}

void Game::runLoop() {
    while (!WindowShouldClose()) {
        update();
        if (WindowShouldClose()) break;
        render();
    }
}

void Game::startGame() {
    lasers.clear();
    asteroids.clear();
    drops.clear();
    player = Player();
    enemies.clear();
    gameHelper.setPlayerHealthPtr(player.getHealthPtr());
    gameHelper.setTextures(textureManager, player);
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

    if (IsKeyPressed(KEY_I))
        levelManager.runLevel(1);

    player.update();
    camera.target = player.getPosition();
    laserHelper.updateLasers(lasers, (int)player.getPosition().x, (int)player.getPosition().y);
    asteroidHelper.updateAsteroids(asteroids, player.getPosition());
    enemyManager.updateEnemies(player.getPosition());

    gameHelper.handleCollision(lasers, asteroids, player.getRect());
    gameHelper.checkCollisionPlayerDrop(drops, player.getRect());
    gameHelper.checkCollisionLaserPlayerEnemy();

    levelManager.updateCurrentLevel();

    dropHelper.updateDrops(drops);

    player.tryShoot(lasers);
}

void Game::renderPlaying() {
    ui.drawStars(player.getPosition());
    BeginMode2D(camera);

    laserHelper.renderLasers(lasers);
    asteroidHelper.renderAsteroids(asteroids);
    dropHelper.renderDrops(drops);
    enemyManager.renderEnemies();
    player.render();

    EndMode2D();

    ui.draw(player.getHealth(), player.getShield(),
        player.getAmmo(), player.getMaxAmmo(),
        player.getScore(), levelManager.getRemainingLevelTime());

    gameHelper.drawPosition();

    levelManager.drawLevelEndOverlay(screenWidth, screenHeight);
}

void Game::updatePaused() {
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) {
        togglePause();
    }
    ui.updatePauseOverlay(screenWidth, screenHeight);
}

void Game::renderPaused() {
    ui.drawStars(player.getPosition());
    BeginMode2D(camera);

    laserHelper.renderLasers(lasers);
    asteroidHelper.renderAsteroids(asteroids);
    dropHelper.renderDrops(drops);
    player.render();

    EndMode2D();

    ui.draw(player.getHealth(), player.getShield(),
        player.getAmmo(), player.getMaxAmmo(),
        player.getScore(), levelManager.getRemainingLevelTime());

    ui.renderPauseOverlay(screenWidth, screenHeight);
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
    ui.drawStars(player.getPosition());
    DrawText("GAME OVER", screenWidth / 2 - 190, screenHeight / 2 - 80, 60, RED);
    DrawText("R - Restart", screenWidth / 2 - 120, screenHeight / 2 + 10, 30, GRAY);
    DrawText("M - Menu", screenWidth / 2 - 100, screenHeight / 2 + 50, 30, GRAY);
}