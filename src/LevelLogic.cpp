#include "LevelLogic.hpp"

void LevelLogic::initLevel(const LevelData &level) {
	if (asteroidHelper)
		asteroidHelper->setAsteroidCount(level.desiredAsteroidCount);

	if (enemyManager)
		enemyManager->setDesiredCounts(level.desiredEnemiesCount);

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
}

LevelUpdateResult LevelLogic::update(const LevelData& level) {
	LevelUpdateResult result;
	switch (level.type) {
	case LevelType::SurviveAsteroidField:
		result = updateAsteroidFieldLevel(level);
		break;
	case LevelType::DestroyAsteroids:
		result = updateDestroyAsteroidsLevel(level);
		break;
	case LevelType::EnemyInvasion:
		result = updateEnemyInvasionLevel(level);
		break;
	case LevelType::ShipEscort:
		result = updateShipEscortLevel(level);
		break;
	case LevelType::BossFight:
		// updateBossFightLevel(); --- IGNORE ---
		break;
	default:
		result = updateAsteroidFieldLevel(level);
		break;
	}
	return result;
}

void LevelLogic::initAsteroidFieldLevel(const LevelData &level) {
	if (level.type != LevelType::SurviveAsteroidField)
		return;

	*progressAccumulator = -1;
}

LevelUpdateResult LevelLogic::updateAsteroidFieldLevel(const LevelData &level) {
	return LevelUpdateResult::None;
}

void LevelLogic::initDestroyAsteroidsLevel(const LevelData &level) {
}

LevelUpdateResult LevelLogic::updateDestroyAsteroidsLevel(const LevelData &level) {
	*progressAccumulator = asteroidHelper->getDestroyedAsteroidCount();

	if (*progressAccumulator >= level.objectiveCount) {
		return LevelUpdateResult::Completed;
	}
	return LevelUpdateResult::None;
}

void LevelLogic::initEnemyInvasionLevel(const LevelData &level) {
	if (level.type != LevelType::EnemyInvasion)
		return;
}

LevelUpdateResult LevelLogic::updateEnemyInvasionLevel(const LevelData &level) {
    if (level.type != LevelType::EnemyInvasion)
        return LevelUpdateResult::None;

    if (enemyManager->getKilledEnemies() > *progressAccumulator)
        (*progressAccumulator)++;

    if (*progressAccumulator >= level.objectiveCount) {
        return LevelUpdateResult::Completed;
    }
    return LevelUpdateResult::None;
}

void LevelLogic::initShipEscortLevel(const LevelData &level) {
	if (level.type != LevelType::ShipEscort)
		return;

	if (!customShipManager)
		throw std::runtime_error(std::string("customShipManagerNotLoaded"));
	if (!ui)
		throw std::runtime_error(std::string("uiNotLoaded"));

	// zrobic randomowe pocz�tkowe miejsce w promieniu np 500px i randomowy
	// waypoint np 1500-2000px
	customShipManager->addShip({-100.0f, GetScreenHeight() / 2.0f},
							   level.waypoint);

	ui->setArrowDestination(customShipManager->getShipPosition(0));
}

LevelUpdateResult LevelLogic::updateShipEscortLevel(const LevelData &level) {
    if (level.type != LevelType::ShipEscort)
        return LevelUpdateResult::None;

    if (!customShipManager || !ui)
        return LevelUpdateResult::Failed;

    CustomShip *escortShip = customShipManager->getCustomShipPtr(0);

    if (customShipManager->isEmpty() || !escortShip || !escortShip->bAlive)
        return LevelUpdateResult::Failed;

    const Vector2 escortPos = escortShip->position;

    if (!Vector2Equals(escortPos, Vector2{0.0f, 0.0f})) {
        ui->setArrowDestination(escortPos);
    }

    if (escortShip->isDestinationReached()) {
        std::printf("DESTINATION REACHED+++++++++++++++++++++++");
        return LevelUpdateResult::Completed;
    }

    return LevelUpdateResult::None;
}