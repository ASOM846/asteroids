#pragma once
#include <raylib.h>
#include <vector>
#include "button.hpp"
#include "levelManager.hpp"
#include "GameHelper.h"
#include "upgradeShop.hpp"
#include "upgradeSystem.hpp"

enum class eWindowState;
class WindowManager;
class Ui;
class SaveManager;

enum class MenuState {
    Main,
    Levels,
    Settings,
    UpgradeShop
};

class Menu {
public:
    explicit Menu();
    void update();
    void render();

    void setMenuState(MenuState newState);

    void setPointers(WindowManager* game, Ui* ui,
        std::vector<LevelData>* levelData, UpgradeSystem* upgrades, int* currency, SaveManager* saveMgr);
private:
    void renderStars();
    void initButtons(int screenWidth, int screenHeight);
    void renderButtons();
    void updateLayout();

    void updateMainMenu();
    void renderMainMenu();

    void updateLevelsMenu();
    void renderLevelsMenu();
    void renderLevelsGrid();

    void updateSettingsMenu();
    void renderSettingsMenu();

    void updateUpgradeShop();
    void renderUpgradeShop();

    WindowManager* gamePtr = nullptr;
    Ui* uiPtr = nullptr;
    std::vector<LevelData>* levels = nullptr;
    UpgradeSystem* upgradeSystem = nullptr;
    int* currency = nullptr;
    SaveManager* saveManager = nullptr;

    UpgradeShop upgradeShop;


    Vector2 lastMousePos;
    bool buttonsInitialized;
    int cachedScreenWidth;
    int cachedScreenHeight;

    MenuState currentState;
    double nextInputAllowedTime;
    const double levelClickDelaySeconds;

    int currentPage;

    //menu buttons
    NewButton endlessModeButton;
    NewButton levelsButton;
    NewButton upgradesButton;
    NewButton settingsButton;
    NewButton exitButton;
    NewButton backButton;
	NewButton arcadeModeButton;

    NewButton nextPageButton;
    NewButton prevPageButton;
};