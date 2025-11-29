#include "player.h"

Player::Player() : x(GetScreenWidth()/2), y(GetScreenHeight()/2), speed(5.0f), rotation(0.0f),
turnSpeed(3.5f), health(100), shield(0), size(50), vPosition{ x,y },
vx(0.0f), vy(0.0f), thrust(0.18f), friction(0.985f),
maxSpeed(9.0f), shootInterval(0.18f), shootTimer(0.0f), ammo(300),
maxAmmo(500),score(0) {
}

Player::~Player() {}

void Player::update() {
    const float dt = GetFrameTime();

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

    const float half = size * 0.5f;
    const int screenW = GetScreenWidth();
    const int screenH = GetScreenHeight();

    vPosition = { x, y };

    if (shootTimer > 0.0f) shootTimer -= dt;
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

void Player::tryShoot(std::vector<Laser>& lasers) {
    if (IsKeyDown(KEY_SPACE) && shootTimer <= 0.0f
            && ammo > 0) {
        ammo--;
        std::cout << ammo << std::endl;
        const float rad = rotation * (std::numbers::pi_v<float> / 180.0f);
        const float dirX = std::sin(rad);
        const float dirY = -std::cos(rad);

        const float muzzleOffset = size * 0.55f;
        const float spawnX = x + dirX * muzzleOffset;
        const float spawnY = y + dirY * muzzleOffset;

        lasers.emplace_back(spawnX, spawnY, 
            dirX, dirY, 12.0f, true);
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

