#pragma once

#include "image.h"
#include "utils.h"

enum class ButtonType
{
    // Actions
    CLEAR,
    LOAD,
    SAVE,

    // Tools
    PENCIL,
    ERASER,
    LINE,
    RECTANGLE,
    TRIANGLE,

    // Colors
    COLOR_BLACK,
    COLOR_WHITE,
    COLOR_PINK,
    COLOR_YELLOW,
    COLOR_RED,
    COLOR_BLUE,
    COLOR_CYAN,
    COLOR_GREEN
};

class Button
{
public:
    Image icon;
    Vector2 pos;
    ButtonType type;

    Button() = default;
    Button(const Image& img, const Vector2& p, ButtonType t);

    bool IsMouseInside(const Vector2& mousePosition) const;
    void Render(Image& framebuffer) const;
};
