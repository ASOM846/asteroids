#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <string>
#include <raylib.h>

class Button {
public:
    Button();
    Button(float x, float y, float width, float height, const std::string& label);
    void Draw() const;
    bool IsClicked() const;
    bool IsHovered() const;
    void SetLabel(const std::string& newLabel);
    void Update();
    void Toggle();
    void SetPosition(float newX, float newY);
    float GetWidth() const;
    float GetHeight() const;
    float GetX() const; 
    float GetY() const;

private:
    float x;
    float y;
    float width;
    float height;
    std::string label;
    Color color;
    bool isToggled;
};

#endif // BUTTON_HPP
