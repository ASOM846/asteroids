#pragma once
#include <cmath>
#include <numbers>
#include <vector>
#include <raylib.h>

#include "player.h"
#include "laser.h"
#include "asteroid.h"
#include "GameHelper.h"
#include "textureManager.h"
#include "drop.h"
#include "menu.h"
#include "ui.h"
#include "enemy.hpp"

enum class GameState {
	Menu,
	Playing,
	Paused,
	GameOver
};

class Game
{
	public:
	Game();
	~Game();

	void setGameState(GameState newState);

private:
	void initialize();
	void shutdown();
	void runLoop();

	void startGame();
	void endGame();

	void update();
	void render();

	void updateMenu();
	void renderMenu();
	void updatePlaying();
	void renderPlaying();
	void updatePaused();
	void renderPaused();
	void updateGameOver();
	void renderGameOver();

	void togglePause();

	int screenWidth;
	int screenHeight;
	
	Player player;
	LaserHelper laserHelper;
	AsteroidHelper asteroidHelper;
	GameHelper gameHelper;
	TextureManager textureManager;
	Ui ui;
	Menu menu;
	DropHelper dropHelper;
	EnemyManager enemyManager;

	Color BcgColour;
	GameState gameState;
	Camera2D camera;
	float playTimeSeconds;
	double playSessionStartTime;
	std::vector<Laser> lasers;
	std::vector<sAsteroid> asteroids;
	std::vector<sDrop> drops;
	std::vector<sEnemy> enemies;
};