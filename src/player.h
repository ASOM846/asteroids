#pragma once
#include <cmath>
#include <numbers>
#include <vector>
#include <raylib.h>
#include <iostream>

#include "laser.h"
#include "textureManager.h"

class UpgradeSystem;

extern TextureManager gTextures;

class Player
{
public:
    Player();
    ~Player();

    void update();
    void render();
    void move();
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
    void heal(int amount);
    void healShield(int amount);
    void increaseAmmo(int amount);
    void increaseScore(int aomunt);
    void registerKill(); // For combo system
    int getComboCount() const { return comboCount; }
    float getComboMultiplier() const;
    void setTexture(Texture2D texture) {
        playerTexture = texture;
    }
    
    void resetWithUpgrades(const UpgradeSystem& upgradeSystem);
    
    float getDamageMultiplier() const { return damageMultiplier; }

private:
    float size;
    float x;
    float y;
    float speed;
    float rotation;
    float turnSpeed;

    int health;
    int shield;

    int maxHealth;
    int maxShield;
    
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

    float damageMultiplier;
    float shieldRegenRate;
    float shieldRegenTimer;
    
    // Combo system
    int comboCount;
    float comboTimer;
    static constexpr float COMBO_TIMEOUT = 3.0f;
    
    // Combo tier thresholds and multipliers
    static constexpr int COMBO_TIER_1_MAX = 5;
    static constexpr int COMBO_TIER_2_MAX = 10;
    static constexpr float COMBO_TIER_1_MULT = 0.1f;
    static constexpr float COMBO_TIER_2_MULT = 0.15f;
    static constexpr float COMBO_TIER_3_MULT = 0.05f;
    static constexpr float COMBO_TIER_1_BASE = 1.0f;
    static constexpr float COMBO_TIER_2_BASE = 1.4f;

    Texture2D playerTexture;
    Vector2 vPosition;
};