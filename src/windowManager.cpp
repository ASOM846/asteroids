#include "windowManager.hpp"

WindowManager::WindowManager()
    : screenWidth(1280),
    screenHeight(720),

    menu(this, &ui, &levels),

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
    if (windowState == eWindowState::Playing) {
        startGame();
    }
    else if (windowState == eWindowState::GameOver) {
        endGame();
    }
}

void WindowManager::runLevel(int levelNumber) {
    game.runLevel(levelNumber);
}

void WindowManager::initialize() {
    InitWindow(screenWidth, screenHeight, "Asteroids Game");
    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    game.initialize();

    levelManager.setOnLevelComplete([this]() {
        this->setWindowState(eWindowState::Menu);
        });
}

void WindowManager::shutdown() {
    game.shutdown();
    // UWAGA: W wykorzystywanej wersji raylib istnieje błąd w obsłudze default font:
    // UnloadFontDefault() zwalnia obrazy glyphów, które wskazują na wcześniej zwolnioną pamięć,
    // co powoduje naruszenie pamięci podczas CloseWindow().
    // Aby tego uniknąć bez modyfikacji raylib, pomijamy wywołanie CloseWindow() i pozwalamy
    // procesowi natywnemu zakończyć kontekst graficzny razem z procesem aplikacji.
    // Jeśli zaktualizujesz raylib do wersji poprawiającej ten bug, przywróć wywołanie CloseWindow().
    //
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
    case eWindowState::Playing:  game.update(); break;
    case eWindowState::Paused:   updatePaused(); break;
    case eWindowState::GameOver: updateGameOver(); break;
    }
}

void WindowManager::render() {
    BeginDrawing();
    ClearBackground(BcgColour);

    switch (windowState) {
    case eWindowState::Menu:     renderMenu(); break;
    case eWindowState::Playing:  game.render(); break;
    case eWindowState::Paused:   renderPaused(); break;
    case eWindowState::GameOver: renderGameOver(); break;
    }

    EndDrawing();
}

void WindowManager::updateMenu() {
    menu.update();
    if (windowState == eWindowState::Playing) {
        startGame();
    }
}

void WindowManager::renderMenu() {
    menu.render();
}


