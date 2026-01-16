#pragma once
#include "asteroid.h"
#include "customShip.hpp"
#include "drop.h"
#include "enemy.hpp"
#include "player.h"
#include <functional>
#include <raylib.h>
#include <string>
#include <vector>

enum class LevelType {
  SurviveAsteroidField,
  DestroyAsteroids,
  EnemyInvasion,
  ShipEscort,
  BossFight,
  ProtectBase,
  Pursuit
};

inline const char* LevelTypeToString(LevelType t) noexcept {
    switch (t) {
    case LevelType::SurviveAsteroidField: return "Asteroid Field";
    case LevelType::DestroyAsteroids:     return "Destroy Asteroids";
    case LevelType::EnemyInvasion:        return "Enemy Invasion";
    case LevelType::ShipEscort:           return "Ship Escort";
    case LevelType::BossFight:            return "Boss Fight";
    case LevelType::ProtectBase:          return "Protect Base";
    case LevelType::Pursuit:              return "Pursuit";
    default:                              return "Unknown";
    }
}

struct LevelData {
    LevelType type;
    bool isUnlocked;
    int levelNumber;
    int objectiveCount;
    float duration;
    int desiredAsteroidCount;
    std::array<int, 3> desiredEnemiesCount;
    std::string objective;
    Vector2 waypoint;
};


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

  void setPointers(std::vector<LevelData> *levelVec, DropHelper *dh, Player *p,
                   AsteroidHelper *ah, CustomShipManager *csm, EnemyManager *em,
                   Ui *pUi, std::vector<sDrop> *dropVec,
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
  }

  void loadLevelsToMemory();

private:
  LevelData currentLevel;

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

  void initAsteroidFieldLevel(const LevelData &level);
  void updateAsteroidFieldLevel();

  void initDestroyAsteroidsLevel(const LevelData &level);
  void updateDestroyAsteroidsLevel();

  void initEnemyInvasionLevel(const LevelData &level);
  void updateEnemyInvasionLevel();

  void initShipEscortLevel(const LevelData &level);
  void updateShipEscortLevel();

  void initBossFightLevel(const LevelData &level);
  void updateBossFightLevel();

  std::function<void()> returnToMenuCallback;
  bool levelEnding = false;
  bool isLevelCompleted = false;
  bool isLevelLose = false;
  float endTimer = 0.0f;
  float endDuration = 2.0f;
  int progressAccumulator = 0;
};
