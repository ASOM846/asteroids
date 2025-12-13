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
#include "button.hpp"
#include "levelManager.hpp"
#include "customShip.hpp"

enum class eWindowState {
	Menu,
	Playing,
	Paused,
	GameOver
};

class WindowManager
{
	public:
	WindowManager();
	~WindowManager();

	void setWindowState(eWindowState newState);
	void runLevel(int levelNumber);

	void togglePause();
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
	LevelManager levelManager;
	CustomShipManager customShipManager;

	Color BcgColour;
	eWindowState windowState;
	Camera2D camera;
	std::vector<Laser> lasers;
	std::vector<sAsteroid> asteroids;
	std::vector<sDrop> drops;
	std::vector<sEnemy> enemies;
	std::vector<LevelData> levels;
	std::vector<CustomShip> customShips;

	/*
	* GameState gameState;
	friend class GameHelper;

	*/

};