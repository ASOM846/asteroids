#include "gameState.hpp"

GameState::GameState() {}
GameState::~GameState() {}

void GameState::update() {
	updateCached();

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

void GameState::render() {
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

void GameState::updateGameOver() {
    if (IsKeyPressed(KEY_R)) {
        startGame();
    }
    else if (IsKeyPressed(KEY_M)) {
        gameState = GameState::Menu;
    }
}

void GameState::renderGameOver() {
    ui.drawStars(player.getPosition());
    DrawText("GAME OVER", screenWidth / 2 - 190, screenHeight / 2 - 80, 60, RED);
    DrawText("R - Restart", screenWidth / 2 - 120, screenHeight / 2 + 10, 30, GRAY);
    DrawText("M - Menu", screenWidth / 2 - 100, screenHeight / 2 + 50, 30, GRAY);
}

void GameState::updateCached()
{
    cacheScreenWidth = GetScreenWidth();
    cacheScreenHeight = GetScreenHeight();
}
