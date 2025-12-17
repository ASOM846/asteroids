#pragma once
#include <raylib.h>
#include <unordered_map>
#include <stdexcept>

enum class TextureId {
    Player,
    MBrownBig1,
    MBrownBig2,
    MBrownBig3,
    MBrownBig4,
    MGreyBig1,
    MGreyBig2,
    MGreyBig3,
    MGreyBig4,
    UpHealth,
    UpShield,
    UpAmmo,
    Enemy1,
    Enemy2,
    FriendlyShip1,
};

class TextureManager {
public:
    TextureManager() = default;
    ~TextureManager() { unloadAll(); }

    void loadAll() {
        load(TextureId::Player, "assets/player.png");
        load(TextureId::MBrownBig1, "assets/meteorBrown_big1.png");
        load(TextureId::MBrownBig2, "assets/meteorBrown_big2.png");
        load(TextureId::MBrownBig3, "assets/meteorBrown_big3.png");
        load(TextureId::MBrownBig4, "assets/meteorBrown_big4.png");
        load(TextureId::MGreyBig1, "assets/meteorGrey_big1.png");
        load(TextureId::MGreyBig2, "assets/meteorGrey_big2.png");
        load(TextureId::MGreyBig3, "assets/meteorGrey_big3.png");
        load(TextureId::MGreyBig4, "assets/meteorGrey_big4.png");
        load(TextureId::UpHealth, "assets/health.png");
        load(TextureId::UpShield, "assets/shield.png");
        load(TextureId::UpAmmo, "assets/ammo.png");
        load(TextureId::Enemy1, "assets/enemy1.png");
        load(TextureId::Enemy2, "assets/enemy2.png");
        load(TextureId::FriendlyShip1, "assets/friendlyShip1.png");
    }

    const Texture2D& get(TextureId id) const {
        auto it = textures.find(id);
        if (it == textures.end()) throw std::runtime_error("Brak tekstury");
        return it->second;
    }

    void unloadAll() {
        for (auto& kv : textures) {
            UnloadTexture(kv.second);
        }
        textures.clear();
    }

private:
    void load(TextureId id, const char* path) {
        Texture2D tex = LoadTexture(path);
        if (tex.id == 0) throw std::runtime_error(std::string("Nie uda�o si� wczyta�: ") + path);
        textures.emplace(id, tex);
    }

    std::unordered_map<TextureId, Texture2D> textures;
};