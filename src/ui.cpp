#include "ui.h"
#include <string>
#include <algorithm>

Ui::Ui() {}
Ui::~Ui() {}

void Ui::draw(int health, int shield, int ammo, int maxAmmo, int score) {
    const int screenW = GetScreenWidth();
    const int screenH = GetScreenHeight();

    // Matrix-like greens
    Color matrixGlow = {80, 255, 120, 220};
    Color shadowCol  = {0, 0, 0, 160};

    // Bottom band baseline
    const int bandHeight = 140;
    const int bandY = screenH - bandHeight;

    // LEFT: vertical HP and Shield, placed on bottom-left, no frames
    const int leftX = 36;
    int y = bandY + 18;

    const int maxHealth = 100;
    int clampedHp = std::clamp(health, 0, maxHealth);
    float hpPct = std::clamp(clampedHp / (float)maxHealth, 0.0f, 1.0f);

    // HP label + value (less bright)
    int lblFont = 14;
    int valFont = 26;
    DrawText("HP", leftX, y, lblFont, matrixGlow);
    std::string hpVal = std::to_string(clampedHp);
    int hpValW = MeasureText(hpVal.c_str(), valFont);
    DrawText(hpVal.c_str(), leftX + 240 - hpValW, y - 6, valFont, matrixGlow);
    y += lblFont + 8;

    // HP bar (filled only, no outline) - dimmer fill
    int barW = 220;
    int barH = 12;
    int hpFillW = (int)(hpPct * barW + 0.5f);
    // subtle background (very dim) for readability, not a frame
    DrawRectangle(leftX, y, barW, barH, {0,0,0,80});
    DrawRectangle(leftX, y, hpFillW, barH, matrixGlow);
    y += barH + 14;

    // SHIELD (less bright)
    const int maxShield = 100;
    int clampedSh = std::clamp(shield, 0, maxShield);
    float shPct = std::clamp(clampedSh / (float)maxShield, 0.0f, 1.0f);

    DrawText("SHIELD", leftX, y, lblFont, matrixGlow);
    std::string shVal = std::to_string(clampedSh);
    int shValW = MeasureText(shVal.c_str(), valFont-6);
    DrawText(shVal.c_str(), leftX + 240 - shValW, y - 4, valFont-6, matrixGlow);
    y += lblFont + 8;

    int shBarH = 10;
    int shFillW = (int)(shPct * barW + 0.5f);
    DrawRectangle(leftX, y, barW, shBarH, {0,0,0,80});
    DrawRectangle(leftX, y, shFillW, shBarH, matrixGlow);

    // CENTER: big ammo number centered on bottom
    int safeMaxAmmo = (maxAmmo > 0) ? maxAmmo : 1;
    int curAmmo = std::clamp(ammo, 0, safeMaxAmmo);
    std::string ammoStr = std::to_string(curAmmo);
    int ammoFontBig = 84;
    int ammoW = MeasureText(ammoStr.c_str(), ammoFontBig);
    int ammoX = (screenW - ammoW) / 2;
    int ammoY = screenH - 92;

    // shadow for depth (no frame)
    DrawText(ammoStr.c_str(), ammoX + 2, ammoY + 2, ammoFontBig, shadowCol);
    DrawText(ammoStr.c_str(), ammoX, ammoY, ammoFontBig, matrixGlow);

    // small max ammo beside it (dim)
    std::string ammoMaxStr = "/" + std::to_string(safeMaxAmmo);
    DrawText(ammoMaxStr.c_str(), ammoX + ammoW + 10, ammoY + (ammoFontBig/2) - 14, 20, matrixGlow);

    // Score moved to RIGHT: slightly smaller and still visible
    std::string scoreNum = std::to_string(score);
    int scoreNumFont = 36; // reduced from 48
    int scoreNumW = MeasureText(scoreNum.c_str(), scoreNumFont);
    int rightPadding = 36;
    int scoreNumX = screenW - rightPadding - scoreNumW;
    int scoreNumY = bandY + 26;

    // shadow + glow for prominence (slightly toned)
    DrawText(scoreNum.c_str(), scoreNumX + 3, scoreNumY + 3, scoreNumFont, shadowCol);
    DrawText(scoreNum.c_str(), scoreNumX, scoreNumY, scoreNumFont, matrixGlow);

    // label "SCORE" above the number, centered to it (slightly smaller)
    std::string scoreLabel = "SCORE";
    int scoreLabelFont = 12;
    int scoreLabelW = MeasureText(scoreLabel.c_str(), scoreLabelFont);
    int scoreLabelX = scoreNumX + (scoreNumW - scoreLabelW) / 2;
    DrawText(scoreLabel.c_str(), scoreLabelX + 1, scoreNumY - scoreLabelFont - 6 + 1, scoreLabelFont, shadowCol);
    DrawText(scoreLabel.c_str(), scoreLabelX, scoreNumY - scoreLabelFont - 6, scoreLabelFont, matrixGlow);

    // Optional tiny HUD accents: thin separators in matrix color, very subtle
    DrawLine(20, bandY + 2, screenW - 20, bandY + 2, {0,50,20,100});
    DrawLine(20, screenH - 2, screenW - 20, screenH - 2, {0,50,20,100});
}