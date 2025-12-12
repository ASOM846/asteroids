#include "levelManager.hpp"

LevelManager::LevelManager() {}

LevelManager::~LevelManager() {}

void LevelManager::reset() {
    currentLevel = LevelData{};
    levelRunning = false;
    currentLevelTime = 0.0f;
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
				//initEnemyInvasionLevel(level); 

                break;
            case LevelType::ShipEscort:
                //initShipEscortLevel(level); --- IGNORE ---
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
            if (onLevelComplete)
                onLevelComplete();
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
        // updateEnemyInvasionLevel(); --- IGNORE ---
        break;
    case LevelType::ShipEscort:
        // updateShipEscortLevel(); --- IGNORE ---
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

    levels->push_back(level1);
    levels->push_back(level2);
}

void LevelManager::drawLevelEndOverlay(int screenWidth, int screenHeight)
{
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
    if (player)
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
    std::cout << "Asteroid Field Level running. Time: " << static_cast<int>(currentLevelTime) << " / " << currentLevel.duration << "\n";

}