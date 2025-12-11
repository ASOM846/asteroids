#include "menu.h"
#include "ui.h"
#include "game.h"
#include <string>

Menu::Menu(Game* game, Ui* ui, std::vector<LevelData>* levelData)
        : gamePtr(game),
            uiPtr(ui),
            lastMousePos{0.0f, 0.0f},
            levels(levelData),
            buttonsInitialized(false),
            cachedScreenWidth(0),
            cachedScreenHeight(0),
            currentState(MenuState::Main),
            nextInputAllowedTime(0.0),
            levelClickDelaySeconds(0.2) {}

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
    const int btnW = 200;
    const int btnH = 50;
    const int spacing = 20;
    const int count = 4;
    int totalHeight = count * btnH + (count - 1) * spacing;
    const int verticalOffset = 80;
    int startY = h / 2 - totalHeight / 2 + verticalOffset;
    int x = w / 2 - btnW / 2;

    quickStartButton = Button(x, startY + (btnH + spacing) * 0, btnW, btnH, "Quick Start");
    levelsButton     = Button(x, startY + (btnH + spacing) * 1, btnW, btnH, "Level Selection");
    settingsButton   = Button(x, startY + (btnH + spacing) * 2, btnW, btnH, "Settings");
    exitButton       = Button(x, startY + (btnH + spacing) * 3, btnW, btnH, "Exit");
    backButton       = Button(w / 2 - btnW / 2, h - 80, btnW, btnH, "Menu");
}

void Menu::renderButtons() {
    if (!buttonsInitialized) {
        return;
    }

    for (const auto& button : {quickStartButton,
        levelsButton, settingsButton, exitButton}) {
        button.Draw();
    }
}

//menu state 

void Menu::updateMainMenu() {
    updateLayout();
    const bool isInputLocked = GetTime() < nextInputAllowedTime;

    if (isInputLocked) return;

    if (quickStartButton.IsClicked())
    {
        gamePtr->setGameState(GameState::Playing);
        nextInputAllowedTime = GetTime() + levelClickDelaySeconds;
    }
    else if (levelsButton.IsClicked())
    {
        currentState = MenuState::Levels;
        nextInputAllowedTime = GetTime() + levelClickDelaySeconds;
    }
    else if (settingsButton.IsClicked())
    {
        currentState = MenuState::Settings;
        nextInputAllowedTime = GetTime() + levelClickDelaySeconds;
    }
    else if (exitButton.IsClicked())
    {
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

    DrawText("WYBOR POZIOMU", w / 2 - 200, h / 2 - 120, 50, WHITE);
    renderLevelsGrid();
}

void Menu::renderLevelsGrid() {
    if (levels == nullptr || levels->empty()) {
        int w = GetScreenWidth();
        int h = GetScreenHeight();
        DrawText("Brak poziomow do wyswietlenia", w / 2 - 180, h / 2, 24, LIGHTGRAY);
        return;
    }

    const int columns = 4;
    const int tileWidth = 180;
    const int tileHeight = 110;
    const int spacing = 20;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    int rows = (static_cast<int>(levels->size()) + columns - 1) / columns;
    int gridWidth = columns * tileWidth + (columns - 1) * spacing;
    int gridHeight = rows * tileHeight + (rows - 1) * spacing;

    int startX = screenWidth / 2 - gridWidth / 2;
    int startY = screenHeight / 2 - gridHeight / 2 + 20;

    const bool isInputLocked = GetTime() < nextInputAllowedTime;

    for (size_t i = 0; i < levels->size(); ++i) {
        const LevelData& level = levels->at(i);
        int row = static_cast<int>(i) / columns;
        int col = static_cast<int>(i) % columns;

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
            && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            fillColor = Fade(SKYBLUE, 0.6f);
            borderColor = WHITE;
            gamePtr->setGameState(GameState::Playing);
            gamePtr->runLevel(level.levelNumber);
            nextInputAllowedTime = GetTime() + levelClickDelaySeconds;
        }

        DrawRectangleRounded(tileRect, 0.1f, 4, fillColor);
        DrawRectangleRoundedLines(tileRect, 0.1f, 4, borderColor);

        int textX = static_cast<int>(tileX) + 12;
        int textY = static_cast<int>(tileY) + 16;

        std::string levelLabel = "Poziom " + std::to_string(level.levelNumber);
        DrawText(levelLabel.c_str(), textX, textY, 20, WHITE);

        DrawText(TextFormat("Trudnosc: %d", level.difficulty), textX, textY + 26, 18, LIGHTGRAY);
        DrawText(TextFormat("Cel: %s", level.objective.c_str()), textX, textY + 46, 18, GRAY);
        DrawText(TextFormat("Typ: %d", static_cast<int>(level.type)), textX, textY + 66, 16, SKYBLUE);
    }

    //back to menu button
    backButton.SetPosition(screenWidth / 2 - static_cast<int>(backButton.GetWidth() / 2), screenHeight - 80);
    backButton.Draw();
    if (!isInputLocked && backButton.IsClicked()) {
        currentState = MenuState::Main;
        nextInputAllowedTime = GetTime() + levelClickDelaySeconds;
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

    DrawText("USTAWIENIA", w / 2 - 150, h / 2 - 120, 50, WHITE);
    DrawText("Kliknij, aby powrocic do menu", w / 2 - 220, h - 100, 20, GRAY);
}