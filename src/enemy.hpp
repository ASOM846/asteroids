#pragma once
#include "laser.h"
#include "player.h"
#include "textureManager.h"
#include "entityUtils.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <numbers>
#include <raylib.h>
#include <vector>

enum class EnemyType {
	Basic,
	Fast,
	Tank,
};

struct sEnemy {
	Vector2 position;
	bool active;

	EnemyType type;

	int health;
	int maxHealth;
	int radius;

	int minDistanceToTarget;
	int distanceToTarget;
	float speed;
	const Texture2D *texture = nullptr;
	float rotation;

	float shootInterval;
	float shootTimer;

	Vector2 getPosition() const {
		return position;
	}
	int getRadius() const {
		return radius;
	}

	sEnemy(Vector2 pos, EnemyType pType, const Texture2D *tex = nullptr)
		: position(pos), active(true), type(pType), health(0), maxHealth(0), radius(0),
		  minDistanceToTarget(0), distanceToTarget(0), speed(0.0f),
		  texture(tex), rotation(0.0f), shootInterval(0.0f), shootTimer(0.0f) {
		struct Params {
			int health;
			int radius;
			float speed;
			float shootInterval;
			int minDistanceToTarget;
			const Texture2D *tex;
		};

		static constexpr Params table[] = {
			/* Basic */ {30, 20, 2.0f, 1.5f, 200, nullptr},
			/* Fast  */ {40, 15, 4.0f, 0.8f, 100, nullptr},
			/* Tank  */ {100, 30, 2.0f, 2.5f, 300, nullptr}};

		const auto idx = static_cast<size_t>(type);
		const auto &p = table[idx];
		health = p.health;
		maxHealth = p.health;
		radius = p.radius;
		speed = p.speed;
		minDistanceToTarget = p.minDistanceToTarget;
		texture = p.tex;

		// Init shooting timers
		shootInterval = p.shootInterval;
		shootTimer = 0.0f;
	}

	void update(const Vector2 &targetPos, std::vector<Laser> &lasers) {
		if (shootTimer > 0.0f) {
			shootTimer -= GetFrameTime();
			if (shootTimer < 0.0f)
				shootTimer = 0.0f;
		}

		Vector2 dir = {targetPos.x - position.x, targetPos.y - position.y};
		float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
		distanceToTarget = static_cast<int>(len);

		if (len > 0.0001f && distanceToTarget > minDistanceToTarget) {
			dir.x /= len;
			dir.y /= len;
			position.x += dir.x * speed;
			position.y += dir.y * speed;

			float angleRad = atan2f(dir.y, dir.x);
			rotation = angleRad * (180.0f / 3.14159274101257324f);
		}
		shoot(targetPos, lasers);
	}

	void render(const int drawHealthBars) const {
		if (!active)
			return;
		if (texture && texture->id != 0) {
			Rectangle src{0, 0, (float)texture->width, (float)texture->height};
			Rectangle dst{position.x, position.y, radius * 2.0f, radius * 2.0f};
			Vector2 origin{static_cast<float>(radius),
						   static_cast<float>(radius)};
			DrawTexturePro(*texture, src, dst, origin, rotation, WHITE);
		} else {
			DrawCircle((int)position.x, (int)position.y, (float)radius, BLUE);
		}
		if (drawHealthBars)
			eUtils::drawBarNoBg(position.x, position.y, health, maxHealth);
	}

	void takeDamage(int dmg) {
		health -= dmg;
		if (health <= 0) {
			active = false;
		}
	}

	void shoot(Vector2 targetPos, std::vector<Laser> &lasers) {
		if (shootTimer > 0.0f)
			return;

		Vector2 dir = {targetPos.x - position.x, targetPos.y - position.y};
		float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
		if (len > 0.0001f) {
			dir.x /= len;
			dir.y /= len;

			lasers.emplace_back(position.x, position.y, dir.x, dir.y, 6.0f,
								false);
			shootTimer = shootInterval;
		}
	}
};

class EnemyManager {
  public:
	EnemyManager()
		: killedEnemies(0), activeCounts{{0, 0, 0}}, desiredCounts{{0, 0, 0}},
		  texManager(nullptr), enemies(nullptr), lasers(nullptr) {
	}
	~EnemyManager() {
	}

	void setPointers(TextureManager *tm, std::vector<sEnemy> *enemyList,
					 std::vector<Laser> *laserList, Player *pPlayer) {
		texManager = tm;
		enemies = enemyList;
		lasers = laserList;
		player = pPlayer;
		recomputeActiveCounts();
	}

	void updateEnemies(const Vector2 &playerPos) {
		if (!enemies)
			return;

		for (auto &e : *enemies) {
			if (e.active)
				e.update(playerPos, *lasers);
		}

		auto it = std::remove_if(enemies->begin(), enemies->end(),
								 [](const sEnemy &en) { return !en.active; });
		const size_t removed =
			static_cast<size_t>(std::distance(it, enemies->end()));
		if (removed > 0) {
			killedEnemies += static_cast<int>(removed);
			enemies->erase(it, enemies->end());
		}

		recomputeActiveCounts();
		ensureDesiredCounts();
	}

	void renderEnemies() {
		if (!enemies)
			return;
		for (const auto &e : *enemies) {
			e.render(drawHealthBars);
		}
	}

	void resetEnemies(void) {
		if (enemies)
			enemies->clear();
		killedEnemies = 0;
		activeCounts.fill(0);
		desiredCounts.fill(0);
	}

	// Return number of active enemies for given type
	int getActiveCount(EnemyType type) const {
		const auto idx = static_cast<size_t>(type);
		if (idx >= activeCounts.size())
			return 0;
		return activeCounts[idx];
	}

	std::array<int, 3> getActiveCounts() const {
		return activeCounts;
	}

	void setDesiredCount(EnemyType type, int count) {
		if (count < 0)
			count = 0;
		desiredCounts[static_cast<size_t>(type)] = count;
	}

	void setDesiredCounts(const std::array<int, 3> &counts) {
		for (size_t i = 0; i < desiredCounts.size(); ++i) {
			desiredCounts[i] = counts[i] < 0 ? 0 : counts[i];
		}
	}

	int getDesiredCount(EnemyType type) const {
		return desiredCounts[static_cast<size_t>(type)];
	}

	int getKilledEnemies() const {
		return killedEnemies;
	}

	std::array<int, 3> getDesiredCounts() const {
		return desiredCounts;
	}

	void generateEnemy(const EnemyType type) {
		if (!enemies || !texManager)
			return;

		const int screenW = GetScreenWidth();
		const int screenH = GetScreenHeight();

		const float halfW = screenW / 2.0f;
		const float halfH = screenH / 2.0f;

		const float visibleMinX = player->getPosition().x - halfW;
		const float visibleMaxX = player->getPosition().x + halfW;
		const float visibleMinY = player->getPosition().y - halfH;
		const float visibleMaxY = player->getPosition().y + halfH;

		const float offscreen = 50.0f;

		auto randFloat = [](float min, float max) {
			if (max <= min)
				return min;
			constexpr int precision = 1000;
			const float t =
				(float)GetRandomValue(0, precision) / (float)precision;
			return min + t * (max - min);
		};

		Vector2 pos;
		const int edge = GetRandomValue(1, 4);
		switch (edge) {
		case 1: // left of player view
			pos.x = visibleMinX - offscreen;
			pos.y = randFloat(visibleMinY, visibleMaxY);
			break;
		case 2: // top of player view
			pos.x = randFloat(visibleMinX, visibleMaxX);
			pos.y = visibleMinY - offscreen;
			break;
		case 3: // right of player view
			pos.x = visibleMaxX + offscreen;
			pos.y = randFloat(visibleMinY, visibleMaxY);
			break;
		case 4: // bottom of player view
		default:
			pos.x = randFloat(visibleMinX, visibleMaxX);
			pos.y = visibleMaxY + offscreen;
			break;
		}

		sEnemy enemy(pos, type);

		switch (type) {
		case EnemyType::Basic:
			enemy.texture = &texManager->get(TextureId::Enemy1);
			break;
		case EnemyType::Fast:
			enemy.texture = &texManager->get(TextureId::Enemy1);
			break;
		case EnemyType::Tank:
			enemy.texture = &texManager->get(TextureId::Enemy2);
			break;
		}

		enemies->emplace_back(enemy);
		// Update active counts after adding a new enemy
		recomputeActiveCounts();
		std::cout << "Generated enemy of type " << static_cast<int>(type)
				  << " at (" << pos.x << ", " << pos.y << ")\n";
	}

	void setDrawHealthBars(bool draw) {
		drawHealthBars = draw;
	}

  private:
	bool drawHealthBars = true;
	int killedEnemies;
	std::array<int, 3> activeCounts{{0, 0, 0}};
	std::array<int, 3> desiredCounts{{0, 0, 0}};

	TextureManager *texManager = nullptr;
	std::vector<sEnemy> *enemies = nullptr;
	std::vector<Laser> *lasers = nullptr;

	Player *player = nullptr;

	void recomputeActiveCounts() {
		activeCounts.fill(0);
		if (!enemies)
			return;
		for (const auto &e : *enemies) {
			if (e.active) {
				const auto idx = static_cast<size_t>(e.type);
				if (idx < activeCounts.size())
					++activeCounts[idx];
			}
		}
	}

	void ensureDesiredCounts() {
		if (!enemies || !texManager)
			return;
		// recompute first to ensure up-to-date
		recomputeActiveCounts();
		for (size_t i = 0; i < desiredCounts.size(); ++i) {
			int desired = desiredCounts[i];
			int current = activeCounts[i];
			while (current < desired) {
				generateEnemy(static_cast<EnemyType>(i));
				++current;
			}
		}
	}
};