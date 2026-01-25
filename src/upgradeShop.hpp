#pragma once
#include <raylib.h>
#include "button.hpp"
#include "upgradeSystem.hpp"

class UpgradeShop {
public:
    UpgradeShop();
    ~UpgradeShop() = default;
    
    void update(int& currency, UpgradeSystem& upgradeSystem);
    void render(int currency, const UpgradeSystem& upgradeSystem);
    
    void initButtons(int screenWidth, int screenHeight);
    
    bool shouldClose() const { return closeRequested; }
    void reset() { closeRequested = false; }
    
private:
    void renderUpgradeCard(const Upgrade* upgrade, int x, int y, int width, int height, 
                          int currency, bool isHovered, bool isSelected);
    
    Vector2 lastMousePos;
    bool buttonsInitialized;
    int cachedScreenWidth;
    int cachedScreenHeight;
    
    bool closeRequested;
    int selectedUpgradeIndex;
    
    NewButton backButton;
    
    static constexpr int CARDS_PER_ROW = 2;
    static constexpr int CARD_WIDTH = 380;
    static constexpr int CARD_HEIGHT = 140;
    static constexpr int CARD_SPACING = 20;
};
