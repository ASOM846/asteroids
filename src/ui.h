#pragma once
#include <raylib.h>
#include <string>

class Ui {
public:
    Ui();
    ~Ui();

    void draw(int health, int shield, 
            int ammo, int maxAmmo, int score);
};