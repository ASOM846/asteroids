#include "levelManager.hpp"
#include <raymath.h>

LevelManager::LevelManager() {}

LevelManager::~LevelManager() {}

void LevelManager::reset() {
    currentLevel = LevelData{};
    levelRunning = false;
    currentLevelTime = 0.0f;
    if (ui) {
        ui->resetAll();
    }
}

void LevelManager::runLevel(int levelNumber) {
    if (!levels) return;
    for (const auto& level : *levels) {
        if (level.levelNumber == levelNumber) {
            currentLevel = level;
            resetCurrentLevelTime();
            setLevelRunning(true);
            switch (level.type) {
            case LevelType::SurviveAsteroidField:
                initAsteroidFieldLevel(level);
                std::cout << "Starting Asteroid Field Level " << level.levelNumber << "\n";
                break;
            case LevelType::EnemyInvasion:
				initEnemyInvasionLevel(level);
                std::cout << "Starting Enemy Invasion Level " << level.levelNumber << "\n";
                break;
            case LevelType::ShipEscort:
                initShipEscortLevel(level);
                break;
            case LevelType::BossFight:
                //initBossFightLevel(level); --- IGNORE ---
                break;
            default:
                initAsteroidFieldLevel(level);
                break;
            }
            break;
        }
    }
}

void LevelManager::updateCurrentLevel()
{
    if (levelEnding)
    {
        endTimer += GetFrameTime();
        if (endTimer >= endDuration)
        {
            // animacja zakończona -> powiadomienie i reset
            if (returnToMenuCallback)
                returnToMenuCallback();
            reset();
            levelEnding = false;
        }
        return;
    }

    if (!levelRunning)
        return;

    if (currentLevelTime >= currentLevel.duration)
    {
        std::cout << "Level " << currentLevel.levelNumber << " completed! Starting end animation...\n";
        levelEnding = true;
        endTimer = 0.0f;
        setLevelRunning(false);
        return;
    }

    currentLevelTime += GetFrameTime();
    switch (currentLevel.type)
    {
    case LevelType::SurviveAsteroidField:
        updateAsteroidFieldLevel();
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

LevelData LevelManager::getCurrentLevelData() {
    return currentLevel;
}

int LevelManager::getCurrentLevelNumber() const {
    return currentLevel.levelNumber;
}

float LevelManager::getCurrentLevelTime() const
{
    return currentLevelTime;
}

bool LevelManager::isLevelRunning() const
{
    return levelRunning;
}

float LevelManager::getRemainingLevelTime() const
{
    return currentLevel.duration - currentLevelTime;
}

void LevelManager::loadLevelsToMemory() {
    LevelData level1;
    level1.levelNumber = 1;
    level1.difficulty = 1;
    level1.type = LevelType::SurviveAsteroidField;
    level1.duration = 60.0f;

    LevelData level2;
    level2.levelNumber = 2;
    level2.difficulty = 1;
    level2.type = LevelType::EnemyInvasion;
    level2.duration = 60.0f;
	level2.objective = "Defeat 5 enemies";
	level2.objectiveCount = 5;

    LevelData level3;
	level3.levelNumber = 3;
	level3.difficulty = 2;
	level3.type = LevelType::DestroyAsteroids;
	level3.duration = 60.0f;
	level3.objective = "Destroy 10 Asteroids";
	level3.objectiveCount = 10;

    LevelData level4;
	level4.levelNumber = 4;
	level4.difficulty = 2;
	level4.type = LevelType::ShipEscort;
	level4.duration = 90.0f;
	level4.objective = "Escort the ship safely";

	LevelData level5;
	level5.levelNumber = 5;
	level5.difficulty = 3;
	level5.type = LevelType::BossFight;
	level5.duration = 120.0f;
	level5.objective = "Defeat the Boss";

    LevelData level6;
	level6.levelNumber = 6;
	level6.difficulty = 3;
	level6.type = LevelType::EnemyInvasion;
	level6.duration = 90.0f;
	level6.objective = "Defeat 15 enemies";
	level6.objectiveCount = 15;

    levels->push_back(level1);
    levels->push_back(level2);
	levels->push_back(level3);
	levels->push_back(level4);
	levels->push_back(level5);
	levels->push_back(level6);
}

void LevelManager::drawLevelEndOverlay(int screenWidth, int screenHeight) {
    if (!levelEnding)
        return;

    float t = endTimer / endDuration;
    if (t > 1.0f)
        t = 1.0f;

    // ciemny fade
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, t * 0.85f));

    // Główny napis
    const char *mainMsg = "POZIOM UKONCZONY";
    int mainSize = 56;
    int mainW = MeasureText(mainMsg, mainSize);
    DrawText(mainMsg, screenWidth / 2 - mainW / 2, screenHeight / 2 - 40, mainSize, GOLD);

    // Dodatkowa informacja (np. wynik gracza)
    std::string info = "Powrot do menu...";
    if (player && currentLevel.type == LevelType::SurviveAsteroidField)
    {
        info = "Wynik: " + std::to_string(player->getScore()); // wymaga, by Player miał getScore()
    }
    int infoSize = 26;
    int infoW = MeasureText(info.c_str(), infoSize);
    DrawText(info.c_str(), screenWidth / 2 - infoW / 2, screenHeight / 2 + 30, infoSize, WHITE);

    // opcjonalny pasek postępu pokazujący "czas do powrotu"
    int barW = 400;
    int barH = 18;
    int bx = screenWidth / 2 - barW / 2;
    int by = screenHeight / 2 + 70;
    DrawRectangleLines(bx, by, barW, barH, WHITE);
    DrawRectangle(bx + 2, by + 2, (int)((barW - 4) * t), barH - 4, SKYBLUE);
}

void LevelManager::resetCurrentLevelTime()
{
    currentLevelTime = 0.0f;
}

void LevelManager::initAsteroidFieldLevel(const LevelData &level)
{
    if (level.type != LevelType::SurviveAsteroidField)
        return;

    if (asteroidHelper)
    {
        asteroidHelper->setAsteroidCount(level.difficulty * 10);
    }
}

void LevelManager::updateAsteroidFieldLevel()
{
    if (currentLevel.type != LevelType::SurviveAsteroidField)
        return;
    std::cout << "Asteroid Field Level running. Time: " 
        << static_cast<int>(currentLevelTime) << " / " << currentLevel.duration << "\n";
}

void LevelManager::initEnemyInvasionLevel(const LevelData &level)
{
    if (currentLevel.type != LevelType::EnemyInvasion)
        return;

    std::array<int, 3> desiredCounts{ {0,0,0} };

	switch (level.difficulty) {
	case 1:
		desiredCounts = { {2, 0, 0} };
		asteroidHelper->setAsteroidCount(2);
		break;
	case 2:
		desiredCounts = { {3, 1, 0} };
		asteroidHelper->setAsteroidCount(3);
		break;
	case 3:
		desiredCounts = { {4, 2, 1} };
		asteroidHelper->setAsteroidCount(4);
		break;
	default:
		desiredCounts = { {2, 0, 0} };
		asteroidHelper->setAsteroidCount(6);
		break;
	}

    enemyManager->setDesiredCounts(desiredCounts);
}

void LevelManager::updateEnemyInvasionLevel()
{
    if (currentLevel.type != LevelType::EnemyInvasion)
        return;
    
	if (enemyManager->getKilledEnemies() >= currentLevel.objectiveCount)
    {
		std::cout << "Killed required enemies for level completion!\n";
		levelEnding = true;
        return;
    }

    if (enemyManager->getKilledEnemies() > currentLevel.currentCount) {
        currentLevel.currentCount++;
		std::cout << "Current killed enemies: " << enemyManager->getKilledEnemies() << " / " << currentLevel.objectiveCount << "\n";
    }
    std::cout << "Enemy Invasion Level running. Time: " << static_cast<int>(currentLevelTime) << " / " << currentLevel.duration << "\n";
}

void LevelManager::initShipEscortLevel(const LevelData& level)
{
    if (currentLevel.type != LevelType::ShipEscort)
        return;

    if (!customShipManager) throw std::runtime_error(std::string("customShipManagerNotLoaded"));
    if (!ui ) throw std::runtime_error(std::string("uiNotLoaded"));

    customShipManager->addShip({ -100.0f, GetScreenHeight() / 2.0f },
		{ GetScreenWidth() + 100000.0f, 100000.0f });

    ui->setArrowDestination(customShipManager->getShipPosition(0));    
}

void LevelManager::updateShipEscortLevel(){
    if(currentLevel.type != LevelType::ShipEscort)
        return;

    if (!customShipManager || !ui) {
        return;
    }

    const Vector2 escortPos = customShipManager->getShipPosition(0);

    // When there is an escort ship, keep the HUD arrow locked to it.
    if (!Vector2Equals(escortPos, Vector2{0.0f, 0.0f})) {
        ui->setArrowDestination(escortPos);
    } else {
        ui->clearArrowDestination();
    }
}