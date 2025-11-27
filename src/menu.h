#pragma once
#include <raylib.h>

enum class GameState;
class Game;

class Menu {
public:
    explicit Menu(Game* game);
    void update();
    void render();
private:
    Game* gamePtr;
};