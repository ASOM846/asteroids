#pragma once
#include "levels.hpp"
#include "asteroid.h"
#include "customShip.hpp"
#include "enemy.hpp"
#include "ui.h"
#include <raylib.h>

enum class LevelUpdateResult {
  None,
  Completed,
  Failed
};

class LevelLogic {
public:
  LevelLogic() = default;
  ~LevelLogic() = default;

  void initLevel(const LevelData &level);
  LevelUpdateResult update(const LevelData &level);

  void initAsteroidFieldLevel(const LevelData &level);
  LevelUpdateResult updateAsteroidFieldLevel(const LevelData &level);

  void initDestroyAsteroidsLevel(const LevelData &level);
  LevelUpdateResult updateDestroyAsteroidsLevel(const LevelData &level);

  void initEnemyInvasionLevel(const LevelData &level);
  LevelUpdateResult updateEnemyInvasionLevel(const LevelData &level);

  void initShipEscortLevel(const LevelData &level);
  LevelUpdateResult updateShipEscortLevel(const LevelData &level);

  void initBossFightLevel(const LevelData &level);
  LevelUpdateResult updateBossFightLevel(const LevelData &level);

  void setPointers(AsteroidHelper *ah, CustomShipManager *csm, EnemyManager *em,
                   Ui *pUi, int *progressAccumulator) {
    asteroidHelper = ah;
    customShipManager = csm;
    enemyManager = em;
    ui = pUi;
    this->progressAccumulator = progressAccumulator;
  }

private:
  AsteroidHelper *asteroidHelper = nullptr;
  CustomShipManager *customShipManager = nullptr;
  EnemyManager *enemyManager = nullptr;
  Ui *ui = nullptr;
  int *progressAccumulator = nullptr;
};