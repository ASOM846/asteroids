#pragma once
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <iostream>
#include "textureManager.h"

struct CustomShip {
	CustomShip(Texture2D texture, Vector2 initialPosition,
		Vector2 destinationTarget)
		: velocity{ 0.0f, 0.0f },
		position{ initialPosition },
		destination{ destinationTarget },
		rotation(0.0f),
		size(50.0f),
		health(100),
		shield(50),
		texture{ texture }{
	}

	void update() {
		const Vector2 toDestination{ destination.x - position.x, destination.y - position.y };
		const float distance = Vector2Length(toDestination);
		if (distance > 0.1f) {
			constexpr float speed = 5.0f;
			const Vector2 direction = Vector2Scale(Vector2Normalize(toDestination), speed);
			velocity = direction;
			rotation = atan2f(velocity.y, velocity.x) * RAD2DEG + 90.0f; // align texture with velocity; tweak offset if ship graphic points another way
			position.x += velocity.x;
			position.y += velocity.y;
		}
		else {
			position = destination;
			velocity = { 0.0f, 0.0f };
		}
		// std::cout << "CustomShip Position: (" << static_cast<int>(position.x) 
		// 	<< ", " << static_cast<int>(position.y) << ")\n";

		std::printf("CustomShip HP: %d, Shield: %d\n", health, shield);
	}


	void render() {
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

	Vector2 velocity;
	Vector2 position;
	Vector2 destination;

	Rectangle rect;

	float rotation;
	float size;
	int health;
	int shield;
	Texture2D texture{};
};

class CustomShipManager {
public:
	CustomShipManager() = default;
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
		for (auto& ship : *ships) {
			ship.update();
		}
	}

	void renderShips() {
		if (ships == nullptr) {
			return;
		}

		for (auto& ship : *ships) {
			ship.render();
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

private:
	std::vector<CustomShip>* ships = nullptr;
	TextureManager* textureManager = nullptr;
};