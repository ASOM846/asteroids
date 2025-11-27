#include "ui.h"
#include "ui.h"
#include <string>

Ui::Ui() {}
Ui::~Ui() {}

void Ui::draw(int health, int shield, int ammo) {
    // Health
    DrawText(("HP: " + std::to_string(health)).c_str(), 20, 20, 20, DARKGREEN);

    const int maxHealth = 100;
    const int barX = 20;
    const int barY = 50;
    const int barW = 200;
    const int barH = 20;

    DrawRectangleLines(barX, barY, barW, barH, DARKGREEN);
    int fillW = (health > 0 ? (health * barW) / maxHealth : 0);
    DrawRectangle(barX + 1, barY + 1, fillW - 2 < 0 ? 0 : fillW - 2, barH - 2, GREEN);

    // Shield
    DrawText(("SH: " + std::to_string(shield)).c_str(), 20, 80, 20, BLUE);

    const int maxShield = 100;
    const int shieldBarY = barY + barH + 10;

    DrawRectangleLines(barX, shieldBarY, barW, barH, BLUE);
    int shieldFillW = (shield > 0 ? (shield * barW) / maxShield : 0);
    DrawRectangle(barX + 1, shieldBarY + 1, shieldFillW - 2 < 0 ? 0 : shieldFillW - 2, barH - 2, SKYBLUE);

    // Ammo (green like HP)
    const int maxAmmo = 100;
    const int ammoBarY = shieldBarY + barH + 10;

    DrawText(("AM: " + std::to_string(ammo)).c_str(), 20, ammoBarY, 20, DARKGREEN);
    DrawRectangleLines(barX, ammoBarY, barW, barH, DARKGREEN);
    int ammoFillW = (ammo > 0 ? (ammo * barW) / maxAmmo : 0);
    DrawRectangle(barX + 1, ammoBarY + 1, ammoFillW - 2 < 0 ? 0 : ammoFillW - 2, barH - 2, GREEN);

    // FPS
    DrawText(("FPS: " + std::to_string(GetFPS())).c_str(), 120, 20, 20, DARKGREEN);
}