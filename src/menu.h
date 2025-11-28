#pragma once
#include <raylib.h>
#include "ui.h"

enum class GameState;
class Game;

class Menu {
public:
    explicit Menu(Game* game, Ui* ui);
    void update();
    void render();

private:
    void renderStars();

    Game* gamePtr;
    Ui* uiPtr;
};