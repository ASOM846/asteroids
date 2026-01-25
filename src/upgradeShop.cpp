#include "upgradeShop.hpp"
#include <string>

UpgradeShop::UpgradeShop()
    : lastMousePos{0.0f, 0.0f},
      buttonsInitialized(false),
      cachedScreenWidth(0),
      cachedScreenHeight(0),
      closeRequested(false),
      selectedUpgradeIndex(-1) {
}

void UpgradeShop::update(int& currency, UpgradeSystem& upgradeSystem) {
    lastMousePos = GetMousePosition();
    
    if (!IsWindowReady()) return;
    
    int currentWidth = GetScreenWidth();
    int currentHeight = GetScreenHeight();
    
    if (!buttonsInitialized || currentWidth != cachedScreenWidth || 
        currentHeight != cachedScreenHeight) {
        initButtons(currentWidth, currentHeight);
        cachedScreenWidth = currentWidth;
        cachedScreenHeight = currentHeight;
        buttonsInitialized = true;
    }
    
    // Handle back button
    if (backButton.IsClicked()) {
        closeRequested = true;
        return;
    }
    
    // Handle upgrade purchases
    const auto& upgrades = upgradeSystem.getAllUpgrades();
    int startY = 150;
    // Note: rows calculation kept for potential future use
    // int rows = (upgrades.size() + CARDS_PER_ROW - 1) / CARDS_PER_ROW;
    
    for (size_t i = 0; i < upgrades.size(); ++i) {
        int row = i / CARDS_PER_ROW;
        int col = i % CARDS_PER_ROW;
        
        int totalWidth = CARDS_PER_ROW * CARD_WIDTH + (CARDS_PER_ROW - 1) * CARD_SPACING;
        int startX = (currentWidth - totalWidth) / 2;
        
        int x = startX + col * (CARD_WIDTH + CARD_SPACING);
        int y = startY + row * (CARD_HEIGHT + CARD_SPACING);
        
        Rectangle cardRect = {
            (float)x, (float)y, 
            (float)CARD_WIDTH, (float)CARD_HEIGHT
        };
        
        if (CheckCollisionPointRec(lastMousePos, cardRect)) {
            selectedUpgradeIndex = i;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                upgradeSystem.purchaseUpgrade(upgrades[i].type, currency);
            }
        }
    }
}

void UpgradeShop::render(int currency, const UpgradeSystem& upgradeSystem) {
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    
    // Background
    DrawRectangle(0, 0, w, h, Fade(BLACK, 0.8f));
    
    // Title
    const char* title = "UPGRADE SHOP";
    int titleSize = 50;
    int titleWidth = MeasureText(title, titleSize);
    DrawText(title, w / 2 - titleWidth / 2, 40, titleSize, GOLD);
    
    // Currency display
    std::string currencyText = "Credits: " + std::to_string(currency);
    DrawText(currencyText.c_str(), w / 2 - 100, 100, 30, SKYBLUE);
    
    // Render upgrade cards
    const auto& upgrades = upgradeSystem.getAllUpgrades();
    int startY = 150;
    
    for (size_t i = 0; i < upgrades.size(); ++i) {
        int row = i / CARDS_PER_ROW;
        int col = i % CARDS_PER_ROW;
        
        int totalWidth = CARDS_PER_ROW * CARD_WIDTH + (CARDS_PER_ROW - 1) * CARD_SPACING;
        int startX = (w - totalWidth) / 2;
        
        int x = startX + col * (CARD_WIDTH + CARD_SPACING);
        int y = startY + row * (CARD_HEIGHT + CARD_SPACING);
        
        bool isHovered = (selectedUpgradeIndex == static_cast<int>(i));
        renderUpgradeCard(&upgrades[i], x, y, CARD_WIDTH, CARD_HEIGHT, 
                         currency, isHovered, false);
    }
    
    // Back button
    backButton.Draw();
    
    // Instructions
    DrawText("Click on an upgrade to purchase", w / 2 - 200, h - 100, 20, LIGHTGRAY);
}

void UpgradeShop::renderUpgradeCard(const Upgrade* upgrade, int x, int y, 
                                   int width, int height, int currency,
                                   bool isHovered, bool /* isSelected */) {
    Rectangle cardRect = {(float)x, (float)y, (float)width, (float)height};
    
    Color bgColor = Fade(DARKBLUE, 0.7f);
    Color borderColor = Fade(SKYBLUE, 0.8f);
    
    if (isHovered) {
        bgColor = Fade(DARKBLUE, 0.9f);
        borderColor = SKYBLUE;
    }
    
    bool canAfford = (currency >= upgrade->getCurrentCost());
    bool maxedOut = !upgrade->canUpgrade();
    
    if (maxedOut) {
        bgColor = Fade(DARKGREEN, 0.5f);
        borderColor = Fade(GREEN, 0.6f);
    } else if (!canAfford) {
        bgColor = Fade(MAROON, 0.5f);
        borderColor = Fade(RED, 0.6f);
    }
    
    // Draw card background
    DrawRectangleRounded(cardRect, 0.1f, 8, bgColor);
    DrawRectangleRoundedLines(cardRect, 0.1f, 8, 2, borderColor);
    
    int textX = x + 15;
    int textY = y + 15;
    
    // Upgrade name
    DrawText(upgrade->name.c_str(), textX, textY, 24, WHITE);
    
    // Level indicator
    std::string levelText = "Level: " + std::to_string(upgrade->level) + 
                           "/" + std::to_string(upgrade->maxLevel);
    DrawText(levelText.c_str(), textX, textY + 30, 18, LIGHTGRAY);
    
    // Description
    DrawText(upgrade->description.c_str(), textX, textY + 55, 16, GRAY);
    
    // Cost or status
    if (maxedOut) {
        DrawText("MAX LEVEL", textX, textY + 80, 20, GREEN);
    } else {
        std::string costText = "Cost: " + std::to_string(upgrade->getCurrentCost()) + " credits";
        Color costColor = canAfford ? GOLD : RED;
        DrawText(costText.c_str(), textX, textY + 80, 20, costColor);
    }
    
    // Progress bar
    float progress = (float)upgrade->level / (float)upgrade->maxLevel;
    int barWidth = width - 30;
    int barHeight = 8;
    int barX = x + 15;
    int barY = y + height - 20;
    
    DrawRectangle(barX, barY, barWidth, barHeight, Fade(GRAY, 0.3f));
    DrawRectangle(barX, barY, (int)(barWidth * progress), barHeight, SKYBLUE);
}

void UpgradeShop::initButtons(int screenWidth, int screenHeight) {
    int btnWidth = 200;
    int btnHeight = 50;
    backButton = NewButton(
        screenWidth / 2 - btnWidth / 2,
        screenHeight - 60,
        btnWidth,
        btnHeight,
        "Back to Menu"
    );
}
