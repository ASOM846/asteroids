#pragma once
#include <raylib.h>
#include <algorithm>
#include "textureManager.h"

enum class DropType {
    Health,
    Shield,
    Ammo
};

struct sDrop {
    float x;
    float y;
    bool active;
    DropType type;
    const Texture2D* texture;


    inline static const TextureManager* sTexMgr = nullptr;
    static void setTextureManager(const TextureManager* tm) { sTexMgr = tm; }

    sDrop(float px, float py, DropType pType, const Texture2D* pTexture = nullptr)
        : x(px), y(py), active(true), type(pType),
        texture(pTexture ? pTexture : textureForType(pType)) {
    }

    ~sDrop() {

    }

    static const Texture2D* textureForType(DropType dt) {
        if (!sTexMgr) return nullptr;
        TextureId tid =
            (dt == DropType::Health ? TextureId::UpHealth :
                dt == DropType::Shield ? TextureId::UpShield :
                TextureId::UpAmmo);
        return &sTexMgr->get(tid);
    }

    static DropType randomType() {
        int pick = GetRandomValue(0, 2);
        return (pick == 0 ? DropType::Health :
            pick == 1 ? DropType::Shield :
            DropType::Ammo);
    }

    void update() {
    }

    void render() const {
        if (!active) return;
        if (texture && texture->id != 0) {
            Rectangle src{ 0,0,(float)texture->width,(float)texture->height };
            Rectangle dst{ x, y, (float)texture->width, (float)texture->height };
            Vector2 origin{ (float)texture->width / 2.0f, (float)texture->height / 2.0f };
            DrawTexturePro(*texture, src, dst, origin, 0.0f, WHITE);
        }
        else {
            Color c =
                (type == DropType::Health ? RED :
                    type == DropType::Shield ? BLUE :
                    ORANGE);
            DrawCircle((int)x, (int)y, 12.0f, c);
        }
    }

    DropType getType() { return type; }

    Rectangle getRect() const {
        float w = (texture && texture->id != 0) ? static_cast<float>(texture->width) : 24.0f;
        float h = (texture && texture->id != 0) ? static_cast<float>(texture->height) : 24.0f;
        return Rectangle{ x - w / 2.0f, y - h / 2.0f, w, h };
    }
};

class DropHelper {
public:
    DropHelper() : dropSpawnChance(0.3f) {}

    void setTextureManager(TextureManager& tm) {
        texManager = &tm;
        sDrop::setTextureManager(texManager);
    }

    void setDrops(std::vector<sDrop>* d) {
        drops = d;
    }

    void spawnDrop(std::vector<sDrop>& drops, float x, float y, DropType type) {
        const Texture2D* tex = sDrop::textureForType(type);
        drops.emplace_back(x, y, type, tex);
    }

    void spawnRandomDrop(std::vector<sDrop>& drops, float x, float y) {
        DropType t = sDrop::randomType();
        spawnDrop(drops, x, y, t);
    }

    void updateDrops(std::vector<sDrop>& drops) {
        for (auto& d : drops) d.update();
        auto it = std::remove_if(drops.begin(), drops.end(),
            [](const sDrop& d) { return !d.active; });
        drops.erase(it, drops.end());
    }

    void renderDrops(const std::vector<sDrop>& drops) {
        for (const auto& d : drops) d.render();
    }

    void maybeSpawnDrop(float x, float y) {
    if (!drops) return;
    if (GetRandomValue(0, 99) < (int)(dropSpawnChance * 100.0f)) {
        spawnRandomDrop(*drops, x, y);
    }
}

private:
    float dropSpawnChance;

    std::vector<sDrop>* drops = nullptr;
    const TextureManager* texManager = nullptr;
};