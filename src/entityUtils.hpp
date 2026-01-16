#pragma once
#include <raylib.h>
#include <algorithm>

namespace eUtils {

    inline void drawBarNoBg(float centerX, float centerY,
        int value, int maxValue, float offsetY = 30.0f,
        float barW = 70.0f, float barH = 6.0f,
        Color fillColor = { 30, 255, 120, 220 },
        Color borderColor = { 255, 255, 255, 120 }) {
        if (maxValue <= 0) return;

        const float pct = std::clamp(value / static_cast<float>(maxValue), 0.0f, 1.0f);
        const int fillW = static_cast<int>(barW * pct + 0.5f);

        const float left = centerX - (barW * 0.5f);
        const float top = centerY - (barH * 0.5f) - offsetY;

        if (fillW > 0) {
            DrawRectangle(static_cast<int>(left),
                static_cast<int>(top),
                fillW,
                static_cast<int>(barH),
                Fade(fillColor, fillColor.a / 255.0f));
        }

        DrawRectangleLines(static_cast<int>(left),
            static_cast<int>(top),
            static_cast<int>(barW),
            static_cast<int>(barH),
            Fade(borderColor, borderColor.a / 255.0f));
    }

} // namespace utils