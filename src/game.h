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

	Menu menu;


	Color BcgColour;
	GameState gameState;

	/*
	* GameState gameState;
	friend class GameHelper;
	*/

};