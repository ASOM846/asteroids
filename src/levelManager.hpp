#pragma once
#include "levels.hpp"
#include "LevelLogic.hpp"
#include "drop.h"
#include <functional>
#include <raylib.h>
#include <string>
#include <vector>

class Ui;

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

  bool isEnding() const { return levelEnding; }

  void setLevelLose(bool state) { isLevelLose = state; }

  void initializeModules(AsteroidHelper *ah, CustomShipManager *csm,
                       EnemyManager *em, Ui *pUi) {
    levelLogic.setPointers(ah, csm, em, pUi, &progressAccumulator);
	}
  
  void setPointers(std::vector<LevelData> *levelVec, DropHelper *dh, Player *p,
                 AsteroidHelper *ah, CustomShipManager *csm, EnemyManager *em, Ui *pUi,
                 std::vector<sDrop> *dropVec, std::vector<sEnemy> *enemyVec) {
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

  LevelLogic levelLogic;

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
  void setLevelRunning(bool running) { levelRunning = running; }

  void initLevel(const LevelData& level);



  std::function<void()> returnToMenuCallback;
  bool levelEnding = false;
  bool isLevelCompleted = false;
  bool isLevelLose = false;
  float endTimer = 0.0f;
  float endDuration = 2.0f;
  int progressAccumulator = 0;
};
