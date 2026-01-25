#include "player.h"
#include "upgradeSystem.hpp"

Player::Player()
        : size(50.0f),
            x(GetScreenWidth() / 2.0f),
            y(GetScreenHeight() / 2.0f),
            speed(5.0f),
            rotation(0.0f),
            turnSpeed(3.5f),
            health(100),
            shield(0),
            maxHealth(100),
            maxShield(100),
            ammo(300),
            maxAmmo(500),
            score(0),
            vx(0.0f),
            vy(0.0f),
            thrust(0.18f),
            friction(0.985f),
            maxSpeed(9.0f),
            shootInterval(0.18f),
            shootTimer(0.0f),
            damageMultiplier(1.0f),
            shieldRegenRate(0.0f),
            shieldRegenTimer(0.0f),
            comboCount(0),
            comboTimer(0.0f),
            playerTexture{},
            vPosition{ x, y } {
}

Player::~Player() {}

void Player::update() {
    const float dt = GetFrameTime();

    move();

    vPosition = { x, y };

    if (shootTimer > 0.0f) shootTimer -= dt;
    
    // Shield regeneration
    if (shieldRegenRate > 0.0f) {
        shieldRegenTimer += dt;
        if (shieldRegenTimer >= 1.0f) {
            shieldRegenTimer = 0.0f;
            healShield((int)shieldRegenRate);
        }
    }
    
    // Combo timer
    if (comboCount > 0) {
        comboTimer -= dt;
        if (comboTimer <= 0.0f) {
            comboCount = 0;
        }
    }
}

void Player::render() {
    if (playerTexture.id == 0) {
        const Rectangle rect = getRect();
        const Vector2 origin = { rect.width / 2.0f, rect.height / 2.0f };
        DrawRectanglePro(rect, origin, rotation, GREEN);
        return;
    }

    const Rectangle src = { 0.0f, 0.0f, (float)playerTexture.width, (float)playerTexture.height };

    const Rectangle dst = { x, y, size, size };

    const Vector2 origin = { dst.width / 2.0f, dst.height / 2.0f };

    const float texRotation = rotation;

    DrawTexturePro(playerTexture, src, dst, origin, texRotation, WHITE);
}

void Player::move()
{
    if (IsKeyDown(KEY_A)) rotation -= turnSpeed;
    if (IsKeyDown(KEY_D)) rotation += turnSpeed;

    const float rad = rotation * (std::numbers::pi_v<float> / 180.0f);
    const float dirX = std::sin(rad);
    const float dirY = -std::cos(rad);

    if (IsKeyDown(KEY_W)) {
        vx += dirX * thrust;
        vy += dirY * thrust;
    }
    
    if (IsKeyDown(KEY_S)) {
        vx -= dirX * thrust * 0.6f;
        vy -= dirY * thrust * 0.6f;
    }

    vx *= friction;
    vy *= friction;

    const float speedMag = std::sqrt(vx * vx + vy * vy);
    if (speedMag > maxSpeed) {
        const float inv = 1.0f / speedMag;
        vx *= maxSpeed * inv;
        vy *= maxSpeed * inv;
    }

    x += vx;
    y += vy;
}

void Player::tryShoot(std::vector<Laser>& lasers) {
    if (IsKeyDown(KEY_SPACE) && shootTimer <= 0.0f
        && ammo > 0) {
        ammo--;
        const float dt = GetFrameTime();
        const float rad = rotation * (std::numbers::pi_v<float> / 180.0f);
        const float dirX = std::sin(rad);
        const float dirY = -std::cos(rad);

        const float muzzleOffset = size * 0.55f;
        const float spawnX = x + dirX * muzzleOffset;
        const float spawnY = y + dirY * muzzleOffset;

        lasers.emplace_back(spawnX, spawnY,
            dirX, dirY, 12.0f, true);

        lasers.back().vx += vx * dt;
        lasers.back().vy += vy * dt;

        shootTimer = shootInterval;
    }
}

void Player::increaseAmmo(int amount) {
    if (amount <= 0) return;
    ammo += amount;
    if (ammo > maxAmmo) ammo = maxAmmo;
}

void Player::increaseScore(int amount)  {
    if(amount <= 0 ) return;
    score += amount;
}

void Player::takeDamage(int amount) {
    if (amount <= 0) return;

    if (shield > 0) {
        const int absorbed = std::min(shield, amount);
        shield -= absorbed;
        amount -= absorbed;
    }

    if (amount > 0) {
        health -= amount;
        if (health < 0) health = 0;
    }
}

void Player::heal(int amount)
{
    if (amount <= 0) return;
    health += amount;
    if (health > maxHealth) health = maxHealth;
}

void Player::healShield(int amount)
{
    if (amount <= 0) return;
    shield += amount;
    if (shield > maxShield) shield = maxShield;
}

void Player::applyUpgrades(const UpgradeSystem& upgradeSystem) {
    // Apply stat bonuses
    maxHealth = 100 + upgradeSystem.getMaxHealthBonus();
    maxShield = 100 + upgradeSystem.getMaxShieldBonus();
    maxAmmo = 500 + upgradeSystem.getMaxAmmoBonus();
    
    // Apply multipliers
    float speedMult = upgradeSystem.getSpeedMultiplier();
    thrust = 0.18f * speedMult;
    maxSpeed = 9.0f * speedMult;
    
    float turnMult = upgradeSystem.getTurnSpeedMultiplier();
    turnSpeed = 3.5f * turnMult;
    
    float fireRateMult = upgradeSystem.getFireRateMultiplier();
    shootInterval = 0.18f / fireRateMult;
    
    damageMultiplier = upgradeSystem.getDamageMultiplier();
    shieldRegenRate = upgradeSystem.getShieldRegenRate();
    
    // Restore health, shield, and ammo to max after applying upgrades
    health = maxHealth;
    shield = maxShield;
    ammo = maxAmmo;
}

void Player::registerKill() {
    comboCount++;
    comboTimer = COMBO_TIMEOUT;
    
    // Award bonus score based on combo
    int bonusScore = comboCount * 10;
    increaseScore(bonusScore);
}

float Player::getComboMultiplier() const {
    if (comboCount <= 1) return 1.0f;
    if (comboCount <= 5) return 1.0f + (comboCount - 1) * 0.1f; // Up to 1.4x
    if (comboCount <= 10) return 1.4f + (comboCount - 5) * 0.15f; // Up to 2.15x
    return 2.15f + (comboCount - 10) * 0.05f; // Max grows slowly after 10
}

