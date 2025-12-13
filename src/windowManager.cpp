#include "windowManager.hpp"

WindowManager::WindowManager()
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
    windowState(eWindowState::Menu),
    camera{},
    lasers(),
    asteroids(),
    drops(),
    enemies() {
    initialize();
    runLoop();
    shutdown();
}

WindowManager::~WindowManager() {}

void WindowManager::setWindowState(eWindowState newState) {
    if (windowState == newState) return;
    windowState = newState;
    if (windowState == eWindowState::Menu) {
        menu.setMenuState(MenuState::Main);
    }
    if (windowState == eWindowState::Playing) {
        startGame();
    }
    else if (windowState == eWindowState::GameOver) {
        endGame();
    }
}

void WindowManager::runLevel(int levelNumber) {
    levelManager.runLevel(levelNumber);
}

void WindowManager::initialize() {
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
    gameHelper.setUi(&ui);
    gameHelper.setCustomShipManager(&customShipManager);

    enemyManager.setPointers(&textureManager, &enemies, &lasers);

    asteroidHelper.setTextureManager(textureManager);

    dropHelper.setTextureManager(textureManager);

    levelManager.setPointers(&levels, &dropHelper, &player,
        &asteroidHelper, &customShipManager, &enemyManager, &drops, &enemies);
    levelManager.loadLevelsToMemory();
    levelManager.reset();

	customShipManager.setPointers(&customShips);

    ui.initButtons(screenWidth, screenHeight);
    ui.setGame(this);

    levelManager.setOnLevelComplete([this]() {
        this->setWindowState(eWindowState::Menu);
        });
}

void WindowManager::shutdown() {
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

void WindowManager::runLoop() {
    while (!WindowShouldClose()) {
        update();
        if (WindowShouldClose()) break;
        render();
    }
}

void WindowManager::startGame() {
    lasers.clear();
    asteroidHelper.resetAsteroids(asteroids);
    drops.clear();
    player = Player();
	enemyManager.resetEnemies();
    customShipManager.reset();
    gameHelper.setPlayerHealthPtr(player.getHealthPtr());
    gameHelper.setTextures(textureManager, player);
    windowState = eWindowState::Playing;
}

void WindowManager::endGame() {
    windowState = eWindowState::GameOver;
}

void WindowManager::togglePause() {
    if (windowState == eWindowState::Playing) windowState = eWindowState::Paused;
    else if (windowState == eWindowState::Paused) windowState = eWindowState::Playing;
}

void WindowManager::update() {
    gameHelper.checkForFullScreen();

    switch (windowState) {
    case eWindowState::Menu:     updateMenu(); break;
    case eWindowState::Playing:  updatePlaying(); break;
    case eWindowState::Paused:   updatePaused(); break;
    case eWindowState::GameOver: updateGameOver(); break;
    }
}

void WindowManager::render() {
    BeginDrawing();
    ClearBackground(BcgColour);

    switch (windowState) {
    case eWindowState::Menu:     renderMenu(); break;
    case eWindowState::Playing:  renderPlaying(); break;
    case eWindowState::Paused:   renderPaused(); break;
    case eWindowState::GameOver: renderGameOver(); break;
    }

    EndDrawing();
}

void WindowManager::updateMenu() {
    menu.update();
    if (windowState == eWindowState::Playing) {
        startGame();
    }
}

void WindowManager::renderMenu() {
    menu.render();
}

void WindowManager::updatePlaying() {
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
    customShipManager.updateShips();

    gameHelper.handleCollision(lasers, asteroids, player.getRect());
    gameHelper.checkCollisionPlayerDrop(drops, player.getRect());
    gameHelper.checkCollisionLaserPlayerEnemy();

    levelManager.updateCurrentLevel();

    dropHelper.updateDrops(drops);

    player.tryShoot(lasers);
}

void WindowManager::renderPlaying() {
    const Vector2 playerWorldPos = player.getPosition();
    ui.drawStars(playerWorldPos);
    BeginMode2D(camera);

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
        playerWorldPos, friendlyShipPos);

    ui.drawArrowAngled(gameHelper.getAngleBetweenPlayerAndFriendlyShip());

    gameHelper.drawPosition();

    levelManager.drawLevelEndOverlay(screenWidth, screenHeight);
}

void WindowManager::updatePaused() {
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) {
        togglePause();
    }
    ui.updatePauseOverlay(screenWidth, screenHeight);
}

void WindowManager::renderPaused() {
    const Vector2 playerWorldPos = player.getPosition();
    ui.drawStars(playerWorldPos);
    BeginMode2D(camera);

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
        playerWorldPos, friendlyShipPos);

    ui.drawArrowAngled(gameHelper.getAngleBetweenPlayerAndFriendlyShip());

    ui.renderPauseOverlay(screenWidth, screenHeight);
}

void WindowManager::updateGameOver() {
    if (IsKeyPressed(KEY_R)) {
        startGame();
    }
    else if (IsKeyPressed(KEY_M)) {
        windowState = eWindowState::Menu;
    }
}

void WindowManager::renderGameOver() {
    ui.drawStars(player.getPosition());
    DrawText("GAME OVER", screenWidth / 2 - 190, screenHeight / 2 - 80, 60, RED);
    DrawText("R - Restart", screenWidth / 2 - 120, screenHeight / 2 + 10, 30, GRAY);
    DrawText("M - Menu", screenWidth / 2 - 100, screenHeight / 2 + 50, 30, GRAY);
}