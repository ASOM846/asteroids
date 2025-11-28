#pragma once
#include <cmath>
#include <numbers>
#include <vector>
#include <raylib.h>
#include <iostream>

#include "laser.h"
#include "textureManager.h"

extern TextureManager gTextures;

class Player
{
public:
    Player();
    ~Player();

    void update();
    void render();
    void tryShoot(std::vector<Laser>& lasers);

    Rectangle getRect() const { return { x - size / 2.0f, y - size / 2.0f, size, size }; }
    Vector2 getPosition() { return vPosition; }
    int getHealth() const { return health; }
    int getShield() const { return shield; }
    int getAmmo() const { return ammo; }
    int getMaxAmmo() const { return maxAmmo; }
    int* getHealthPtr() { return &health; }
    int* getShieldPtr() { return &shield; }
    int getScore() {return score;}


    void takeDamage(int amount);
    void increaseAmmo(int amount);
    void increaseScore(int aomunt);
    void setTexture(Texture2D texture) {
        playerTexture = texture;
    }

private:
    float size;
    float x;
    float y;
    float speed;
    float rotation;
    float turnSpeed;

    int health;
    int shield;

    int ammo;
    int maxAmmo;

    int score;

    float vx;
    float vy;
    float thrust;
    float friction;
    float maxSpeed;
    float shootInterval;
    float shootTimer;

    Texture2D playerTexture;
    Vector2 vPosition;
};