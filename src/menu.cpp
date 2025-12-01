#include "menu.h"
#include "game.h"

Menu::Menu(Game* game, Ui* ui, std::vector<LevelData>* levelData)
        : gamePtr(game),
            uiPtr(ui),
            lastMousePos{0.0f, 0.0f},
            levels(levelData),
            buttonsInitialized(false),
            cachedScreenWidth(0),
            cachedScreenHeight(0),
            currentState(MenuState::Main) {}

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

    quickStartButton = Button(x, startY + (btnH + spacing) * 0, btnW, btnH, "Szybki Start");
    levelsButton     = Button(x, startY + (btnH + spacing) * 1, btnW, btnH, "Wybor Poziomu");
    settingsButton   = Button(x, startY + (btnH + spacing) * 2, btnW, btnH, "Ustawienia");
    exitButton       = Button(x, startY + (btnH + spacing) * 3, btnW, btnH, "Wyjscie");
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

    if (quickStartButton.IsClicked())
    {
        gamePtr->setGameState(GameState::Playing);
    }
    else if (levelsButton.IsClicked())
    {
        currentState = MenuState::Levels;
    }
    else if (settingsButton.IsClicked())
    {
        currentState = MenuState::Settings;
    }
    else if (exitButton.IsClicked())
    {
        CloseWindow();
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

    // Implement level selection logic here
    // For now, just return to main menu on any button click
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        currentState = MenuState::Main;
    }
}

void Menu::renderLevelsMenu() {
    updateLayout();
    int w = GetScreenWidth();
    int h = GetScreenHeight();

    DrawText("WYBOR POZIOMU", w / 2 - 200, h / 2 - 120, 50, WHITE);
    DrawText("Kliknij, aby powrocic do menu", w / 2 - 220, h - 100, 20, GRAY);
    // Implement level rendering logic here
}

// Settings Menu

void Menu::updateSettingsMenu() {
    updateLayout();

    // Implement settings adjustment logic here
    // For now, just return to main menu on any button click
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
    // Implement settings rendering logic here
}