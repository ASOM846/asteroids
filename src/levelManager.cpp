#include "levelManager.hpp"
#include "ui.h"
#include <raymath.h>

LevelManager::LevelManager() {}

LevelManager::~LevelManager() {}

void LevelManager::reset() {
  currentLevel = LevelData{};
  levelRunning = false;
  isLevelCompleted = false;
  levelEnding = false;
  progressAccumulator = 0;
  resetCurrentLevelTime();
  if (ui) {
    ui->resetAll();
  }
}

void LevelManager::runLevel(int levelNumber) {
  if (!levels)
    return;
  for (const auto &level : *levels) {
    if (level.levelNumber == levelNumber) {
      currentLevel = level;
      // reset per-level state
      resetCurrentLevelTime();
      progressAccumulator = 0;
      setLevelRunning(true);
      switch (level.type) {
      case LevelType::SurviveAsteroidField:
        initAsteroidFieldLevel(level);
        std::cout << "Starting Asteroid Field Level " << level.levelNumber
                  << "\n";
        break;
      case LevelType::DestroyAsteroids:
        initDestroyAsteroidsLevel(level);
        std::cout << "Starting Destroy Asteroids Level " << level.levelNumber
                  << "\n";
        break;
      case LevelType::EnemyInvasion:
        initEnemyInvasionLevel(level);
        std::cout << "Starting Enemy Invasion Level " << level.levelNumber
                  << "\n";
        break;
      case LevelType::ShipEscort:
        initShipEscortLevel(level);
        break;
      case LevelType::BossFight:
        // initBossFightLevel(level); --- IGNORE ---
        break;
      default:
        initAsteroidFieldLevel(level);
        break;
      }
      break;
    }
  }
}

void LevelManager::updateCurrentLevel() {
  if (levelEnding) {
    endTimer += GetFrameTime();
    if (endTimer >= endDuration) {
      if (returnToMenuCallback)
        returnToMenuCallback();
      reset();
      levelEnding = false;
    }
    setLevelUnlockedState(currentLevel.levelNumber + 1, true);
    return;
  }

  if (!levelRunning)
    return;

  if (((currentLevelTime >= currentLevel.duration) && currentLevel.duration > 0) ||
      progressAccumulator >= currentLevel.objectiveCount) {
    std::cout << "Level " << currentLevel.levelNumber
              << " completed! Starting end animation...\n";
    levelEnding = true;
    endTimer = 0.0f;
    setLevelRunning(false);
    return;
  }

  currentLevelTime += GetFrameTime();
  switch (currentLevel.type) {
  case LevelType::SurviveAsteroidField:
    updateAsteroidFieldLevel();
    break;
  case LevelType::DestroyAsteroids:
    updateDestroyAsteroidsLevel();
    break;
  case LevelType::EnemyInvasion:
    updateEnemyInvasionLevel();
    break;
  case LevelType::ShipEscort:
    updateShipEscortLevel();
    break;
  case LevelType::BossFight:
    // updateBossFightLevel(); --- IGNORE ---
    break;
  default:
    updateAsteroidFieldLevel();
    break;
  }
}

const LevelData *LevelManager::getCurrentLevelData() const {
  // Only expose data when a level is active or finishing to keep UI optional
  // path sane
  if (!levelRunning && !levelEnding) {
    return nullptr;
  }
  return &currentLevel;
}

int LevelManager::getCurrentLevelNumber() const {
  return currentLevel.levelNumber;
}

int LevelManager::getProgressAccumulator() const { return progressAccumulator; }

float LevelManager::getCurrentLevelTime() const { return currentLevelTime; }

bool LevelManager::isLevelRunning() const { return levelRunning; }

int LevelManager::getUnlockedLevels() const
{
  if(!levels) 
    return 1;

  int count = 0;

  for(auto l : *levels) {
    if(l.isUnlocked == 1)
      ++count;
  }

  return count;
}

float LevelManager::getRemainingLevelTime() const {
  return currentLevel.duration - currentLevelTime;
}

void LevelManager::loadLevelsToMemory() {
  LevelData level1;
  level1.levelNumber = 1;
  level1.difficulty = 1;
  level1.type = LevelType::SurviveAsteroidField;
  level1.objective = "Survive 60 seconds";
  level1.objectiveCount = 0;
  level1.duration = 60.0f;
  level1.isUnlocked = true;

  LevelData level2;
  level2.levelNumber = 2;
  level2.difficulty = 1;
  level2.type = LevelType::EnemyInvasion;
  level2.duration = 0.0f;
  level2.objective = "Defeat 5 enemies";
  level2.objectiveCount = 5;
  level2.isUnlocked = false;

  LevelData level3;
  level3.levelNumber = 3;
  level3.difficulty = 2;
  level3.type = LevelType::DestroyAsteroids;
  level3.objective = "Destroy 10 Asteroids";
  level3.objectiveCount = 10;
  level3.isUnlocked = false;

  LevelData level4;
  level4.levelNumber = 4;
  level4.difficulty = 2;
  level4.type = LevelType::ShipEscort;
  level4.duration = 90.0f;
  level4.objective = "Escort the ship safely";
  level4.isUnlocked = false;

  LevelData level5;
  level5.levelNumber = 5;
  level5.difficulty = 3;
  level5.type = LevelType::BossFight;
  level5.duration = 120.0f;
  level5.objective = "Defeat the Boss";
  level5.isUnlocked = false;

  LevelData level6;
  level6.levelNumber = 6;
  level6.difficulty = 3;
  level6.type = LevelType::EnemyInvasion;
  level6.duration = 90.0f;
  level6.objective = "Defeat 15 enemies";
  level6.objectiveCount = 15;
  level6.isUnlocked = false;

  LevelData level7;
  level7.levelNumber = 7;
  level7.difficulty = 2;
  level7.type = LevelType::SurviveAsteroidField;
  level7.duration = 120.0f;
  level7.objective = "Survive 120 seconds";
  level7.isUnlocked = false;

  LevelData level8;
  level8.levelNumber = 8;
  level8.difficulty = 3;
  level8.type = LevelType::BossFight;
  level8.duration = 120.0f;
  level8.objective = "Defeat the Boss";
  level8.isUnlocked = false;

  levels->push_back(level1);
  levels->push_back(level2);
  levels->push_back(level3);
  levels->push_back(level4);
  levels->push_back(level5);
  levels->push_back(level6);
  levels->push_back(level7);
  levels->push_back(level8);
}

void LevelManager::setLevelUnlockedState(int levelNumber, bool isUnlocked) {
  if (!levels)
    return;
  for (auto &level : *levels) {
    if (level.levelNumber == levelNumber) {
      level.isUnlocked = isUnlocked;
      break;
    }
  }
}

void LevelManager::setUnlockedLevels(int numberOfUnlockedLevels)  {
  if(!levels)
    return;
  for(auto &level : *levels)  {
    if(level.levelNumber <= numberOfUnlockedLevels)
    level.isUnlocked = true;
  }
}

void LevelManager::drawLevelEndOverlay(int screenWidth, int screenHeight) {
  if (!levelEnding)
    return;

  float t = endTimer / endDuration;
  if (t > 1.0f)
    t = 1.0f;

  DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, t * 0.85f));

  const char *mainMsg = "POZIOM UKONCZONY";
  int mainSize = 56;
  int mainW = MeasureText(mainMsg, mainSize);
  DrawText(mainMsg, screenWidth / 2 - mainW / 2, screenHeight / 2 - 40,
           mainSize, GOLD);

  std::string info = "Powrot do menu...";
  if (player && currentLevel.type == LevelType::SurviveAsteroidField) {
    info = "Wynik: " + std::to_string(player->getScore());
  }
  int infoSize = 26;
  int infoW = MeasureText(info.c_str(), infoSize);
  DrawText(info.c_str(), screenWidth / 2 - infoW / 2, screenHeight / 2 + 30,
           infoSize, WHITE);

  int barW = 400;
  int barH = 18;
  int bx = screenWidth / 2 - barW / 2;
  int by = screenHeight / 2 + 70;
  DrawRectangleLines(bx, by, barW, barH, WHITE);
  DrawRectangle(bx + 2, by + 2, (int)((barW - 4) * t), barH - 4, SKYBLUE);
}

void LevelManager::resetCurrentLevelTime() { currentLevelTime = 0.0f; }

void LevelManager::initAsteroidFieldLevel(const LevelData &level) {
  if (level.type != LevelType::SurviveAsteroidField)
    return;

  if (asteroidHelper) {
    asteroidHelper->setAsteroidCount(level.difficulty * 5);
  }
  progressAccumulator = -1;
}

void LevelManager::updateAsteroidFieldLevel() {
  if (currentLevel.type != LevelType::SurviveAsteroidField)
    return;
  std::cout << "Asteroid Field Level running. Time: "
            << static_cast<int>(currentLevelTime) << " / "
            << currentLevel.duration << "\n";
}

void LevelManager::initDestroyAsteroidsLevel(const LevelData &level) {
  if (currentLevel.type != LevelType::DestroyAsteroids)
    return;

  if (asteroidHelper) {
    asteroidHelper->setAsteroidCount(level.objectiveCount + 5);
    std::printf("Initialized Destroy Asteroids Level with %d asteroids.\n",
                static_cast<int>(level.objectiveCount * 1.1));
  }
}

void LevelManager::updateDestroyAsteroidsLevel() {
  if (currentLevel.type != LevelType::DestroyAsteroids)
    return;

  progressAccumulator = asteroidHelper->getDestroyedAsteroidCount();

  std::cout << "Destroy Asteroids Level running. Time: "
            << static_cast<int>(currentLevelTime) << " / "
            << currentLevel.duration << "\n";
}

void LevelManager::initEnemyInvasionLevel(const LevelData &level) {
  if (currentLevel.type != LevelType::EnemyInvasion)
    return;

  // reset progress accumulator for enemy-kill objectives
  progressAccumulator = 0;
  std::array<int, 3> desiredCounts{{0, 0, 0}};

  switch (level.difficulty) {
  case 1:
    desiredCounts = {{2, 0, 0}};
    asteroidHelper->setAsteroidCount(2);
    break;
  case 2:
    desiredCounts = {{3, 1, 0}};
    asteroidHelper->setAsteroidCount(3);
    break;
  case 3:
    desiredCounts = {{4, 2, 1}};
    asteroidHelper->setAsteroidCount(4);
    break;
  default:
    desiredCounts = {{2, 0, 0}};
    asteroidHelper->setAsteroidCount(6);
    break;
  }

  enemyManager->setDesiredCounts(desiredCounts);
}

void LevelManager::updateEnemyInvasionLevel() {
  if (currentLevel.type != LevelType::EnemyInvasion) {
    return;
  }

  progressAccumulator = enemyManager->getKilledEnemies();

  if (progressAccumulator >= currentLevel.objectiveCount) {
    std::cout << "Killed required enemies for level completion!\n";
    levelEnding = true;
    return;
  }

  if (enemyManager->getKilledEnemies() > currentLevel.currentCount) {
    currentLevel.currentCount++;
    std::cout << "Current killed enemies: " << enemyManager->getKilledEnemies()
              << " / " << currentLevel.objectiveCount << "\n";
  }

  std::cout << "Enemy Invasion Level running. Time: "
            << static_cast<int>(currentLevelTime) << " / "
            << currentLevel.duration << "\n";
}

void LevelManager::initShipEscortLevel(const LevelData &level) {
  if (currentLevel.type != LevelType::ShipEscort)
    return;

  (void)level;

  if (!customShipManager)
    throw std::runtime_error(std::string("customShipManagerNotLoaded"));
  if (!ui)
    throw std::runtime_error(std::string("uiNotLoaded"));

  customShipManager->addShip({-100.0f, GetScreenHeight() / 2.0f},
                             {GetScreenWidth() + 100000.0f, 100000.0f});

  asteroidHelper->setAsteroidCount(10);
  ui->setArrowDestination(customShipManager->getShipPosition(0));
}

void LevelManager::updateShipEscortLevel() {
  if (currentLevel.type != LevelType::ShipEscort)
    return;

  if (!customShipManager || !ui) {
    return;
  }

  const Vector2 escortPos = customShipManager->getShipPosition(0);

  if (!Vector2Equals(escortPos, Vector2{0.0f, 0.0f})) {
    ui->setArrowDestination(escortPos);
  } else {
    ui->clearArrowDestination();
  }
}
