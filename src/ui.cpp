#include "ui.h"
#include "game.h"
#include <string>
#include <algorithm>
#include <cmath>
#include <random>

Ui::Ui() : cachedWidth(0), cachedHeight(0), hasLastPlayerPos(false){
    std::random_device rd;
    rng.seed(rd());
}

Ui::~Ui() {}

void Ui::initStarLayers(int screenW, int screenH) {
    cachedWidth = screenW;
    cachedHeight = screenH;
    starLayers.clear();

    struct LayerConfig {
        int count;
        float parallax;
        Color color;
        float minSize;
        float maxSize;
        float minTwinkle;
        float maxTwinkle;
    };

    const LayerConfig configs[] = {
        {140, 0.18f, {255, 255, 255, 110}, 0.8f, 1.4f, 0.4f, 0.7f},
        { 90, 0.28f, {255, 255, 255, 170}, 1.1f, 2.0f, 0.6f, 1.0f},
        { 60, 0.46f, {255, 255, 255, 230}, 1.6f, 2.6f, 0.8f, 1.2f}
    };

    constexpr float twoPi = 6.28318530718f;
    const float marginX = screenW * 0.25f;
    const float marginY = screenH * 0.25f;

    for (const auto& cfg : configs) {
        starLayers.emplace_back();
        StarLayer& layer = starLayers.back();
        layer.parallax = cfg.parallax;
        layer.color = cfg.color;
        layer.stars.reserve(cfg.count);

        std::uniform_real_distribution<float> sizeDist(cfg.minSize, cfg.maxSize);
        std::uniform_real_distribution<float> twinkleDist(cfg.minTwinkle, cfg.maxTwinkle);
        std::uniform_real_distribution<float> phaseDist(0.0f, twoPi);
        std::uniform_real_distribution<float> posXDist(-marginX, screenW + marginX);
        std::uniform_real_distribution<float> posYDist(-marginY, screenH + marginY);

        for (int i = 0; i < cfg.count; ++i) {
            Star star;
            star.position = { posXDist(rng), posYDist(rng) };
            star.size = sizeDist(rng);
            star.twinkleSpeed = twinkleDist(rng);
            star.twinkleOffset = phaseDist(rng);
            layer.stars.push_back(star);
        }
    }
}

void Ui::draw(int health, int shield, int ammo,
         int maxAmmo, int score, float remainingLevelTime) {
    const int screenW = GetScreenWidth();
    const int screenH = GetScreenHeight();

    // Matrix-like greens
    Color matrixGlow = {80, 255, 120, 220};
    // shadow color (używane przez rysowanie tekstów)
    // przeniesione tutaj, aby nie używać przed deklaracją
    Color shadowCol  = {0, 0, 0, 160};

    // Bottom band baseline (przeniesione wyżej, bo czas używa bandY)
    const int bandHeight = 140;
    const int bandY = screenH - bandHeight;

    // RIGHT SIDE: remaining level time (format MM:SS)
    int timeFont = 20;
    int timeSec = std::max(0, (int)std::ceil(remainingLevelTime));
    int minutes = timeSec / 60;
    int seconds = timeSec % 60;
    char timeBuf[16];
    std::snprintf(timeBuf, sizeof(timeBuf), "%02d:%02d", minutes, seconds);
    std::string timeStr(timeBuf);

    int timeW = MeasureText(timeStr.c_str(), timeFont);
    int timeX = screenW - 36 - timeW;
    int timeY = bandY + 80;

    DrawText("TIME", timeX - 20, timeY - 18, 12, matrixGlow);
    DrawText(timeStr.c_str(), timeX + 2, timeY + 2, timeFont, shadowCol);
    DrawText(timeStr.c_str(), timeX, timeY, timeFont, matrixGlow);

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

void Ui::drawStars(Vector2 playerPos) {
    const int screenW = GetScreenWidth();
    const int screenH = GetScreenHeight();
    if (screenW <= 0 || screenH <= 0) {
        return;
    }

    if (starLayers.empty() || screenW != cachedWidth || screenH != cachedHeight) {
        initStarLayers(screenW, screenH);
    }

    Vector2 delta = {0.0f, 0.0f};
    if (hasLastPlayerPos) {
        delta.x = playerPos.x - lastPlayerPos.x;
        delta.y = playerPos.y - lastPlayerPos.y;
    }
    lastPlayerPos = playerPos;
    hasLastPlayerPos = true;

    const float time = static_cast<float>(GetTime());
    const float marginX = cachedWidth * 0.25f;
    const float marginY = cachedHeight * 0.25f;

    auto randRange = [this](float minVal, float maxVal) {
        std::uniform_real_distribution<float> dist(minVal, maxVal);
        return dist(rng);
    };

    for (auto& layer : starLayers) {
        const float moveX = delta.x * layer.parallax;
        const float moveY = delta.y * layer.parallax;

        for (auto& star : layer.stars) {
            star.position.x -= moveX;
            star.position.y -= moveY;

            if (star.position.x < -marginX) {
                star.position.x = cachedWidth + marginX;
                star.position.y = randRange(-marginY, cachedHeight + marginY);
            }
            else if (star.position.x > cachedWidth + marginX) {
                star.position.x = -marginX;
                star.position.y = randRange(-marginY, cachedHeight + marginY);
            }

            if (star.position.y < -marginY) {
                star.position.y = cachedHeight + marginY;
                star.position.x = randRange(-marginX, cachedWidth + marginX);
            }
            else if (star.position.y > cachedHeight + marginY) {
                star.position.y = -marginY;
                star.position.x = randRange(-marginX, cachedWidth + marginX);
            }

            const float flicker = 0.85f + 0.15f * std::sin(time * star.twinkleSpeed + star.twinkleOffset);
            Color drawColor = layer.color;
            float alpha = drawColor.a * flicker;
            if (alpha < 0.0f) alpha = 0.0f;
            if (alpha > 255.0f) alpha = 255.0f;
            drawColor.a = static_cast<unsigned char>(alpha);

            if (star.position.x >= -1.0f && star.position.x <= cachedWidth + 1.0f &&
                star.position.y >= -1.0f && star.position.y <= cachedHeight + 1.0f) {
                DrawCircleV(star.position, star.size, drawColor);
            }
        }
    }
}

void Ui::setGame(Game* g) {
    game = g;
}

void Ui::initButtons(int sW, int sH)
{
    const float btnW = 200.0f;
    const float btnH = 50.0f;
    const float x = sW / 2.0f - 90.0f;
    const float resumeY = sH / 2.0f + 30.0f;
    const float mainMenuY = sH / 2.0f + 100.0f;

    resumeButton = Button(x, resumeY, btnW, btnH, std::string("WZNAWIAJ"));
    mainMenuButton = Button(x, mainMenuY, btnW, btnH, std::string("MENU GLOWNE"));
}

void Ui::renderPauseOverlay(int sW, int sH)
{
    DrawRectangle(0, 0, sW, sH, Fade(BLACK, 0.6f));
    DrawText("PAUZED", sW / 2 - 90, sH / 2 - 40, 50, WHITE);
	resumeButton.Draw();
	mainMenuButton.Draw();
}

void Ui::updatePauseOverlay(int sW, int sH)
{
	if (resumeButton.IsClicked())
        {
        game->togglePause();
	}
	else if (mainMenuButton.IsClicked())
	{
		game->setGameState(GameState::Menu);
	}
}
