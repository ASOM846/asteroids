#pragma once
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <iostream>
#include "textureManager.h"
#include "laser.h"

struct CustomShip {
	CustomShip(Texture2D texture, Vector2 initialPosition,
		Vector2 destinationTarget)
		: velocity{ 0.0f, 0.0f },
		position{ initialPosition },
		destination{ destinationTarget },
		rotation(0.0f),
		size(50.0f),
		maxHealth(100),
		health(maxHealth),
		shield(50),
		bAlive(true),
		texture{ texture },
		damageTakenTimer(5.0f),
		noDamageTimeToHeal(5.0f),
		autoHealTimer(0.0f),
		autoHealInterval(1.0f) {
	}

	void update() {
		const Vector2 toDestination{ destination.x - position.x, destination.y - position.y };
		const float distance = Vector2Length(toDestination);
		const float dt = GetFrameTime();

		if (distance > 0.1f) {
			constexpr float speed = 5.0f;
			const Vector2 direction = Vector2Scale(Vector2Normalize(toDestination), speed);
			velocity = direction;
			rotation = atan2f(velocity.y, velocity.x) * RAD2DEG + 90.0f;
			position.x += velocity.x;
			position.y += velocity.y;
		}
		else {
			position = destination;
			velocity = { 0.0f, 0.0f };
		}

		if (health == 0) {
			bAlive = false;
		}

		if (damageTakenTimer < noDamageTimeToHeal) damageTakenTimer += dt;
		if (autoHealTimer > 0.0f) autoHealTimer -= dt;

		if (damageTakenTimer >= noDamageTimeToHeal && autoHealTimer <= 0.0f) {
			heal(1);
			autoHealTimer = autoHealInterval;
		}

		std::printf("CustomShip HP: %d, Shield: %d\n", health, shield);
	}


	void render(const bool drawHealthBar) {
		if (texture.id != 0) {
			const Rectangle src = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
			const Rectangle dst = { position.x, position.y, size, size };
			const Vector2 origin = { dst.width / 2.0f, dst.height / 2.0f };
			DrawTexturePro(texture, src, dst, origin, rotation, WHITE);
		}
		else {
			DrawCircle((int)position.x, (int)position.y, size / 2.0f, RED);
		}

	}

	Rectangle getRect() const {
		return Rectangle{
			position.x - size / 2.0f,
			position.y - size / 2.0f,
			size,
			size
		};
	}

	void takeDamage(int amount) {
		if (amount <= 0)
			return;

		damageTakenTimer = 0.0f;
		autoHealTimer = autoHealInterval;

		if (shield > 0) {
			const int absorbed = std::min(shield, amount);
			shield -= absorbed;
			amount -= absorbed;
		}

		if (amount > 0) {
			health -= amount;
			if (health < 0)
				health = 0;
		}
	}

	void heal(int amount) {
		if (amount <= 0)
			return;

		if (health < maxHealth) {
			if (health + amount > maxHealth)
				health = maxHealth;
			else
				health += amount;
		}
	}

	bool isDestinationReached() {
		if (position == destination)
			return true;
		return false;
	}

	Vector2 velocity;
	Vector2 position;
	Vector2 destination;

	Rectangle rect;

	float rotation;
	float size;
	int maxHealth;
	int health;
	int shield;
	bool bAlive;
	Texture2D texture{};

	float damageTakenTimer;
	float noDamageTimeToHeal;
	float autoHealTimer;
	float autoHealInterval;
};

class CustomShipManager {
public:
	CustomShipManager() 
		: drawHealthBars(true) {
	}


	~CustomShipManager() = default;

	void setPointers(std::vector<CustomShip>* cs, TextureManager* tm) {
		ships = cs;
		textureManager = tm;
	}

	void addShip(const Vector2& initialPosition, const Vector2& destinationTarget) {
		if (ships == nullptr) {
			return;
		}
		Texture2D shipTexture = textureManager->get(TextureId::FriendlyShip1);
		ships->emplace_back(shipTexture, initialPosition, destinationTarget);
	}

	void updateShips() {
		if (ships == nullptr) {
			return;
		}
		for (size_t i = 0; i < ships->size(); ) {
			(*ships)[i].update();

			if (!(*ships)[i].bAlive) {
				ships->erase(ships->begin() + i);
			}
			else {
				++i;
			}
		}
	}

	void renderShips() {
		if (ships == nullptr) {
			return;
		}

		for (auto& ship : *ships) {
			ship.render(drawHealthBars);
		}
	}

	void reset() {
		if (ships == nullptr) {
			return;
		}
		ships->clear();
	}

	Vector2 getShipPosition(int index) const {
		if (ships == nullptr || index < 0 ||
			index >= static_cast<int>(ships->size())) {
			return { 0.0f, 0.0f };
		}
		return (*ships)[index].position;
	}

	int getShipHealth(int index) const {
		if (ships == nullptr || index < 0 ||
			index >= static_cast<int>(ships->size())) {
			return 0;
		}

		return (*ships)[index].health;
	}

	bool isEmpty() {
		if (ships == nullptr)
			return true;
		if (ships->size() > 0)
			return false;
		return true;
	}

	CustomShip* getCustomShipPtr(int index) {
		if (ships == nullptr || index < 0
			|| index >= static_cast<int>(ships->size())) {
			return nullptr;
		}

		return &((*ships)[index]);
	}

	void setDrawHealthBars(bool state) { drawHealthBars = state; }
private:
	std::vector<CustomShip>* ships = nullptr;
	TextureManager* textureManager = nullptr;

	bool drawHealthBars;
};