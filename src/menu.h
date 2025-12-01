#pragma once
#include <raylib.h>
#include <vector>
#include "ui.h"
#include "button.hpp"
#include "levelManager.hpp"
#include "GameHelper.h"

enum class GameState;
class Game;

enum class MenuState {
    Main,
    Levels,
    Settings
};
class Menu {
public:
    explicit Menu(Game* game, Ui* ui, 
        std::vector<LevelData>* levelData);
    void update();
    void render();

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

    Game* gamePtr;
    Ui* uiPtr;


    Vector2 lastMousePos;
    std::vector<LevelData>* levels = nullptr;
    bool buttonsInitialized;
    int cachedScreenWidth;
    int cachedScreenHeight;

    MenuState currentState;
    double nextLevelClickAllowedTime;
    const double levelClickDelaySeconds;
    //menu buttons
    Button quickStartButton;
    Button levelsButton;
    Button settingsButton;
    Button exitButton;
};