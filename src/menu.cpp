#include "menu.h"
#include "game.h"

Menu::Menu(Game* game, Ui* ui) : gamePtr(game), uiPtr(ui) {}

void Menu::update() {
    if (IsKeyPressed(KEY_ENTER)) {
        gamePtr->setGameState(GameState::Playing);
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        CloseWindow();
    }
    lastMousePos = GetMousePosition();
}

void Menu::render() {
    int w = GetScreenWidth();
    int h = GetScreenHeight();

    renderStars();

    DrawText("ASTEROIDS", w / 2 - 180, h / 2 - 120, 60, WHITE);
    DrawText("ENTER - Start", w / 2 - 120, h / 2 + 10, 30, GRAY);
    DrawText("ESC - Wyjscie", w / 2 - 120, h / 2 + 50, 30, GRAY);
}

void Menu::renderStars() {
    uiPtr->drawStars(lastMousePos);
}