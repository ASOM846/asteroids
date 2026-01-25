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

#include "game.hpp"

enum class eWindowState {
	Menu,
	Gameplay
};

class WindowManager
{
	public:
	WindowManager();
	~WindowManager();

	void setWindowState(eWindowState newState);
	void runLevel(int levelNumber);
	void startArcadeMode();
	void startQuickStart();

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

	void updateGameOver();
	void renderGameOver();

	int screenWidth;
	int screenHeight;
	
	Game game;
	Menu menu;

	Color BcgColour;
	eWindowState windowState;

	/*
	* GameState gameState;
	friend class GameHelper;

	*/

};