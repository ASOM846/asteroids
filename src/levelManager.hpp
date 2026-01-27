#pragma once
#include "LevelLogic.hpp"
#include "drop.h"
#include "levels.hpp"
#include "levelGenerator.hpp"
#include <functional>
#include <raylib.h>
#include <string>
#include <vector>

class Ui;

enum class Mode {
	Endless,
	Levels
};

class LevelManager {
  public:
	LevelManager();
	~LevelManager();

	void reset();

	void runLevel(int levelNumber);
	void updateCurrentLevel();
	const LevelData *getCurrentLevelData() const;
	int getCurrentLevelNumber() const;
	int getProgressAccumulator() const;
	float getCurrentLevelTime() const;
	float getRemainingLevelTime() const;
	bool isLevelRunning() const;
	int getUnlockedLevels() const;

	void setReturnToMenuCallback(std::function<void()> cb) {
		returnToMenuCallback = cb;
	}
	void setLevelUnlockedState(int levelNumber, bool isUnlocked);
	void setUnlockedLevels(int numberOfUnlockedLevels);

	void drawLevelEndOverlay(int screenWidth, int screenHeight);

	void initializeModules(AsteroidHelper *ah, CustomShipManager *csm,
						   EnemyManager *em, Ui *pUi) {
		levelLogic.setPointers(ah, csm, em, pUi, &progressAccumulator);
	}

	void setPointers(std::vector<LevelData> *levelVec, DropHelper *dh,
					 Player *p, AsteroidHelper *ah, CustomShipManager *csm,
					 EnemyManager *em, Ui *pUi, std::vector<sDrop> *dropVec,
					 std::vector<sEnemy> *enemyVec) {
		levels = levelVec;
		dropHelper = dh;
		player = p;
		asteroidHelper = ah;
		customShipManager = csm;
		enemyManager = em;
		ui = pUi;
		drops = dropVec;
		enemies = enemyVec;

		levelLogic.setPointers(ah, csm, em, pUi, &progressAccumulator);
	}

	void loadLevelsToMemory();

  private:
	LevelData currentLevel;
	Mode currentMode;
    LevelUpdateResult currentLevelResult;

	LevelLogic levelLogic;
	LevelGenerator levelGenerator;

	std::vector<LevelData> *levels;
	DropHelper *dropHelper = nullptr;
	Player *player = nullptr;
	AsteroidHelper *asteroidHelper = nullptr;
	CustomShipManager *customShipManager = nullptr;
	EnemyManager *enemyManager = nullptr;
	Ui *ui = nullptr;
	std::vector<sDrop> *drops = nullptr;
	std::vector<sEnemy> *enemies = nullptr;

	bool levelRunning = false;
	float currentLevelTime = 0.0f;

	void resetCurrentLevelTime();
	void setLevelRunning(bool running) {
		levelRunning = running;
	}

	void initLevel(const LevelData &level);

	std::function<void()> returnToMenuCallback;
	float endTimer = 0.0f;
	float endDuration = 2.0f;
	int progressAccumulator = 0;
};
