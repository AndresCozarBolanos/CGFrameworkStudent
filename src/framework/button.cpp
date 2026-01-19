#include "button.h"

// Constructor
Button::Button(const Image& img, const Vector2& p, ButtonType t)
{
    icon = img;
    pos = p;
    type = t;
}

bool Button::IsMouseInside(const Vector2& m) const
{
    return (m.x >= pos.x && m.x < pos.x + icon.width &&
            m.y >= pos.y && m.y < pos.y + icon.height);
}

void Button::Render(Image& framebuffer) const
{
    framebuffer.DrawImage(icon, (int)pos.x, (int)pos.y);
}
