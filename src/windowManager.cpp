#include "windowManager.hpp"

WindowManager::WindowManager()
    : screenWidth(1280),
    screenHeight(720),

    BcgColour(BLACK),
    windowState(eWindowState::Menu) {
    initialize();
    runLoop();
    shutdown();
}

WindowManager::~WindowManager() {}

void WindowManager::setWindowState(eWindowState newState) {
    if (windowState == newState) return;
    windowState = newState;
    if (windowState == eWindowState::Menu) {
        menu.setMenuState(MenuState::Main);
    }
 }

void WindowManager::runLevel(int levelNumber) {
    game.runLevel(levelNumber);
}

void WindowManager::startArcadeMode() {
    game.startArcadeMode();
}

void WindowManager::initialize() {
    InitWindow(screenWidth, screenHeight, "Asteroids Game");
    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    game.initialize();
    game.setReturnToMenuCallback([this]() {
        this->setWindowState(eWindowState::Menu);
    });

    menu.setPointers(this, &game.ui, &game.levels, &game.upgradeSystem, &game.currency);
}

void WindowManager::shutdown() {
    // CloseWindow(); // wyłączone z powodu błędu w tej wersji raylib
}

void WindowManager::runLoop() {
    while (!WindowShouldClose()) {
        update();
        if (WindowShouldClose()) break;
        render();
    }
}

void WindowManager::update() {
    // fix to window manager.check gameHelper.checkForFullScreen();

    switch (windowState) {
    case eWindowState::Menu:     updateMenu(); break;
    case eWindowState::Gameplay:  game.update(); break;
    }
}

void WindowManager::render() {
    BeginDrawing();
    ClearBackground(BcgColour);

    switch (windowState) {
    case eWindowState::Menu:     renderMenu(); break;
    case eWindowState::Gameplay:  game.render(); break;
    }

    EndDrawing();
}

void WindowManager::updateMenu() {
    menu.update();
}

void WindowManager::renderMenu() {
    menu.render();
}


