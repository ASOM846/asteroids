#include "button.hpp"

Color Green = Color{ 25, 25, 112, 255 }; // Midnight Blue
Color HoverColor = Color{ 72, 61, 139, 255 }; // Dark Slate Blue
Color PressedColor = Color{ 47, 79, 79, 255 }; // Dark Slate Gray

Button::Button() : x(0), y(0), width(100), height(50), label("Button"), color(Green), isToggled(false) {
}

Button::Button(float x, float y, float width, float height, const std::string& label)
    : x(x), y(y), width(width), height(height), label(label), color(Green), isToggled(false) {
}

void Button::Draw() const {
    DrawRectangle(x, y, width, height, color);

    int textWidth = MeasureText(label.c_str(), 20);
    int textX = x + (width - textWidth) / 2;
    int textY = y + (height - 20) / 2;

    DrawText(label.c_str(), textX, textY, 20, WHITE);
}

bool Button::IsClicked() const {
    return CheckCollisionPointRec(GetMousePosition(), { x, y, width, height }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool Button::IsHovered() const {
    return CheckCollisionPointRec(GetMousePosition(), { x, y, width, height });
}

void Button::SetLabel(const std::string& newLabel) {
    label = newLabel;
}

void Button::Update() {
    if (CheckCollisionPointRec(GetMousePosition(), { x, y, width, height })) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            color = PressedColor;
        }
        else {
            color = HoverColor;
        }
    }
    else {
        color = Green;
    }
}

void Button::Toggle() {
    size_t pos = label.find(" : OFF");
    if (pos != std::string::npos) {
        label = label.substr(0, pos);
    }
    pos = label.find(" : ON");
    if (pos != std::string::npos) {
        label = label.substr(0, pos);
    }

    isToggled = !isToggled;
    if (isToggled) {
        SetLabel(label + " : OFF");
    }
    else {
        SetLabel(label + " : ON");
    }
}

void Button::SetPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

float Button::GetWidth() const {
    return width;
}

float Button::GetHeight() const {
    return height;
}

float Button::GetX() const { // Implementacja metody GetX
    return x;
}

float Button::GetY() const { // Implementacja metody GetY
    return y;
}
