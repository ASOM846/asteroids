#pragma once 
#include <vector>
#include "laser.h"
#include "asteroid.h"
#include "drop.h"
#include "enemy.hpp"
#include "player.h"
#include "customShip.hpp"
#include "levelManager.hpp"
#include "textureManager.h"
#include "GameHelper.h"
#include "ui.h"
#include <iostream>
#include <functional>
#include <string>

class GameState {
public:
	GameState() = default;
	~GameState() = default;

	void update();
	void render();

private:

	void runLevel(int levelNumber);
	void togglePause();

	void initialize();
	void shutdown();

	void updateGameOver();
	void renderGameOver();

	void updateCached();

	int cacheScreenWidth;
	int cacheScreenHeight;

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

	Camera2D camera;

	std::vector<Laser> lasers;
	std::vector<sAsteroid> asteroids;
	std::vector<sDrop> drops;
	std::vector<sEnemy> enemies;
	std::vector<LevelData> levels;
	std::vector<CustomShip> customShips;

	friend class GameHelper;
};