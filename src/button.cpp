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

// NewButton
NewButton::NewButton()
    : x(0)
    , y(0)
    , width(100)
    , height(50)
    , label("Button")
    , color(Green)
    , isToggled(false)
    , fillColor(Fade(DARKBLUE, 0.6f))
    , borderColor(Fade(SKYBLUE, 0.9f)) {
}

NewButton::NewButton(float xPos, float yPos, float btnWidth, float btnHeight, const std::string& btnLabel)
    : x(xPos)
    , y(yPos)
    , width(btnWidth)
    , height(btnHeight)
    , label(btnLabel)
    , color(Green)
    , isToggled(false)
    , fillColor(Fade(DARKBLUE, 0.6f))
    , borderColor(Fade(SKYBLUE, 0.9f)) {
}

void NewButton::Draw() const {
    Color currentFillColor = fillColor;
    Color currentBorderColor = borderColor;

    if (IsHovered()) {
        currentFillColor = Fade(SKYBLUE, 0.6f);
        currentBorderColor = WHITE;
    } else {
        currentFillColor = Fade(DARKBLUE, 0.6f);
        currentBorderColor = Fade(SKYBLUE, 0.9f);
    }
    
    Rectangle tileRect{ x, y, width, height };
    DrawRectangleRounded(tileRect, 0.1f, 4, currentFillColor);
    DrawRectangleRoundedLines(tileRect, 0.1f, 4, currentBorderColor);

    int textWidth = MeasureText(label.c_str(), 20);
    int textX = static_cast<int>(x + (width - textWidth) / 2);
    int textY = static_cast<int>(y + (height - 20) / 2);
    DrawText(label.c_str(), textX, textY, 20, WHITE);

}

bool NewButton::IsClicked() const {
    return CheckCollisionPointRec(GetMousePosition(), { x, y, width, height })
        && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool NewButton::IsHovered() const {
    return CheckCollisionPointRec(GetMousePosition(), { x, y, width, height });
}

void NewButton::SetLabel(const std::string& newLabel) {
    label = newLabel;
}

void NewButton::Update() {
    if (CheckCollisionPointRec(GetMousePosition(), { x, y, width, height })) {
        fillColor = Fade(SKYBLUE, 0.6f);
        borderColor = WHITE;
    }
    else {
        fillColor = Fade(DARKBLUE, 0.6f);
        borderColor = Fade(SKYBLUE, 0.9f);
    }
}

void NewButton::Toggle() {
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

void NewButton::SetPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

float NewButton::GetWidth() const {
    return width;
}

float NewButton::GetHeight() const {
    return height;
}

float NewButton::GetX() const {
    return x;
}

float NewButton::GetY() const {
    return y;
}

