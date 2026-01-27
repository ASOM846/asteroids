#include "menu.h"
#include "ui.h"
#include "windowManager.hpp"
#include <string>

Menu::Menu()
        :  lastMousePos{0.0f, 0.0f},
            buttonsInitialized(false),
            cachedScreenWidth(0),
            cachedScreenHeight(0),
            currentState(MenuState::Main),
            nextInputAllowedTime(0.0),
            levelClickDelaySeconds(0.2),
            currentPage(0) {}

void Menu::update() {
    lastMousePos = GetMousePosition();

    switch (currentState) {
    case MenuState::Main:
        updateMainMenu();
        break;
    case MenuState::Levels:
        updateLevelsMenu();
        break;
    case MenuState::Settings:   
        updateSettingsMenu();
        break;
    }
}

void Menu::render() {
    renderStars();
    switch (currentState) {
    case MenuState::Main:
        renderMainMenu();
        break;
    case MenuState::Levels:
        renderLevelsMenu();
        break;
    case MenuState::Settings:
        renderSettingsMenu();
        break;
    }
}

void Menu::setMenuState(MenuState newState) {
    if (currentState == newState)
        return;
    currentState = newState;
    nextInputAllowedTime = GetTime() + levelClickDelaySeconds;
}

void Menu::setPointers(WindowManager* game, Ui* ui,
    std::vector<LevelData>* levelData) {
    gamePtr = game;
    uiPtr = ui;
    levels = levelData;
}

void Menu::renderStars() {
    uiPtr->drawStars(lastMousePos);
}

void Menu::updateLayout() {
    if (!IsWindowReady()) {
        return;
    }

    int currentWidth = GetScreenWidth();
    int currentHeight = GetScreenHeight();

    if (!buttonsInitialized || currentWidth != cachedScreenWidth || currentHeight != cachedScreenHeight) {
        initButtons(currentWidth, currentHeight);
        cachedScreenWidth = currentWidth;
        cachedScreenHeight = currentHeight;
        buttonsInitialized = true;
    }
}

void Menu::initButtons(int w, int h) {
    const int mainBtnW = 220;
    const int mainBtnH = 60;
    const int spacing = 24;
    const int mainBtnCount = 3;
    const int verticalOffset = 60;

    const int columnHeight = mainBtnCount * mainBtnH + (mainBtnCount - 1) * spacing;
    const int startY = h / 2 - columnHeight / 2 + verticalOffset;
    const int x = w / 2 - mainBtnW / 2;

    endlessModeButton = NewButton(x, startY + (mainBtnH + spacing) * 0, mainBtnW, mainBtnH, "Endless");
    arcadeModeButton = NewButton(x, startY + (mainBtnH + spacing) * 1, mainBtnW, mainBtnH, "Arcade");
    levelsButton = NewButton(x, startY + (mainBtnH + spacing) * 2, mainBtnW, mainBtnH, "Levels");

    nextPageButton = NewButton(w - 200, h / 2, 140, 40, ">");
    prevPageButton = NewButton(60, h / 2, 140, 40, "<");

    const int smallBtnW = 140;
    const int smallBtnH = 40;
    const int margin = 24;
    const int bottomGap = 16;

    const int exitX = w - smallBtnW - margin;
    const int exitY = h - smallBtnH - margin;
    const int settingsX = exitX - bottomGap - smallBtnW;

    settingsButton = NewButton(settingsX, exitY, smallBtnW, smallBtnH, "Settings");
    exitButton = NewButton(exitX, exitY, smallBtnW, smallBtnH, "Exit");

    backButton = NewButton(w / 2 - mainBtnW / 2, h - 80, mainBtnW, mainBtnH, "Menu");
}

void Menu::renderButtons() {
    if (!buttonsInitialized) {
        return;
    }

    endlessModeButton.Draw();
    arcadeModeButton.Draw();
    levelsButton.Draw();
    settingsButton.Draw();
    exitButton.Draw();
}

//menu state 

void Menu::updateMainMenu() {
    updateLayout();
    const bool isInputLocked = GetTime() < nextInputAllowedTime;

    if (isInputLocked) {
        return;
    }

    if (endlessModeButton.IsClicked()) {
        gamePtr->setWindowState(eWindowState::Gameplay);
        nextInputAllowedTime = GetTime() + levelClickDelaySeconds;
    }
    else if (arcadeModeButton.IsClicked()) {
        gamePtr->setWindowState(eWindowState::Gameplay);
        nextInputAllowedTime = GetTime() + levelClickDelaySeconds;
    }
    else if (levelsButton.IsClicked()) {
        currentState = MenuState::Levels;
        nextInputAllowedTime = GetTime() + levelClickDelaySeconds;
    }
    else if (settingsButton.IsClicked()) {
        currentState = MenuState::Settings;
        nextInputAllowedTime = GetTime() + levelClickDelaySeconds;
    }
    else if (exitButton.IsClicked()) {
        CloseWindow();
        nextInputAllowedTime = GetTime() + levelClickDelaySeconds;
    }
}

void Menu::renderMainMenu() {
    updateLayout();
    int w = GetScreenWidth();
    int h = GetScreenHeight();


    DrawText("ASTEROIDS", w / 2 - 180, h / 2 - 120, 60, WHITE);
    renderButtons();
}

// Levels Menu

void Menu::updateLevelsMenu() {
    updateLayout();
}

void Menu::renderLevelsMenu() {
    updateLayout();
    int w = GetScreenWidth();
    int h = GetScreenHeight();

    DrawText("LEVEL SELECTION", w / 2 - 250, h / 2 - 200, 50, WHITE);
    renderLevelsGrid();
}

void Menu::renderLevelsGrid() {
    if (levels == nullptr || levels->empty()) {
        int w = GetScreenWidth();
        int h = GetScreenHeight();
        DrawText("No levels to display", w / 2 - 180, h / 2, 24, LIGHTGRAY);
        return;
    }

    const int columns = 4;
    const int tileWidth = 180;
    const int tileHeight = 110;
    const int spacing = 20;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    int rows = 2;
    int gridWidth = columns * tileWidth + (columns - 1) * spacing;
    int gridHeight = rows * tileHeight + (rows - 1) * spacing;

    int startX = screenWidth / 2 - gridWidth / 2;
    int startY = screenHeight / 2 - gridHeight / 2 + 20;

    const bool isInputLocked = GetTime() < nextInputAllowedTime;

    const size_t pageStart = static_cast<size_t>(currentPage) * 8;
    const size_t total = levels->size();
    const size_t pageEnd = std::min(pageStart + 8, total);

    for (size_t i = pageStart; i < pageEnd; ++i) {
        const LevelData& level = levels->at(i);
        size_t localIndex = i - pageStart;
        int row = static_cast<int>(localIndex) / columns;
        int col = static_cast<int>(localIndex) % columns;

        float tileX = static_cast<float>(startX + col * (tileWidth + spacing));
        float tileY = static_cast<float>(startY + row * (tileHeight + spacing));
        Rectangle tileRect{tileX, tileY, static_cast<float>(tileWidth), static_cast<float>(tileHeight)};

        Color fillColor = Fade(DARKBLUE, 0.6f);
        Color borderColor = Fade(SKYBLUE, 0.9f);

        if (CheckCollisionPointRec(lastMousePos, tileRect)) {
            fillColor = Fade(SKYBLUE, 0.6f);
            borderColor = WHITE;
        }

        if (!isInputLocked && CheckCollisionPointRec(lastMousePos, tileRect) 
            && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && level.isUnlocked)
        {
            fillColor = Fade(SKYBLUE, 0.6f);
            borderColor = WHITE;
            gamePtr->setWindowState(eWindowState::Gameplay);
            gamePtr->runLevel(level.levelNumber);
            nextInputAllowedTime = GetTime() + levelClickDelaySeconds;
        }

        DrawRectangleRounded(tileRect, 0.1f, 4, fillColor);
        DrawRectangleRoundedLines(tileRect, 0.1f, 4, borderColor);

        int textX = static_cast<int>(tileX) + 12;
        int textY = static_cast<int>(tileY) + 16;

        std::string levelLabel = "Poziom " + std::to_string(level.levelNumber);
        DrawText(levelLabel.c_str(), textX, textY, 20, WHITE);

        if(level.isUnlocked)    {
            DrawText(TextFormat("%s", LevelTypeToString(level.type)), textX, textY + 26, 18, LIGHTGRAY);
            DrawText(TextFormat("%s", level.objective.c_str()), textX, textY + 46, 15, GRAY);
            //DrawText(TextFormat("Type: %d", static_cast<int>(level.type)), textX, textY + 66, 16, SKYBLUE);
        }
        else    {
            DrawText(TextFormat("Level locked"), textX, textY + 26, 18, LIGHTGRAY);
        }
    }

    //back to menu button
    backButton.SetPosition(screenWidth / 2 - static_cast<int>(backButton.GetWidth() / 2), screenHeight - 80);
    backButton.Draw();
    if (!isInputLocked && backButton.IsClicked()) {
        currentState = MenuState::Main;
        nextInputAllowedTime = GetTime() + levelClickDelaySeconds;
    }

    nextPageButton.Draw();
    if (!isInputLocked && nextPageButton.IsClicked()) {
        if (static_cast<size_t>(currentPage + 1) * 8 < total) {
            currentPage++;
            nextInputAllowedTime = GetTime() + levelClickDelaySeconds;
        }
    }

    prevPageButton.Draw();
    if (!isInputLocked && prevPageButton.IsClicked()) {
        if (currentPage > 0) {
            currentPage--;
            nextInputAllowedTime = GetTime() + levelClickDelaySeconds;
        }
    }
}

// Settings Menu

void Menu::updateSettingsMenu() {
    updateLayout();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        currentState = MenuState::Main;
    }
}

void Menu::renderSettingsMenu() {
    updateLayout();
    int w = GetScreenWidth();
    int h = GetScreenHeight();

    DrawText("SETTINGS", w / 2 - 150, h / 2 - 120, 50, WHITE);
    DrawText("Click to return to menu", w / 2 - 220, h - 100, 20, GRAY);
}