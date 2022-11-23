#include "text.h"

#include <system.h>

Text::Text(const char *new_text, vec2i text_pos, Color text_color, int text_size) {
    text = new_text;
    pos = text_pos;
    color = text_color;
    size = text_size;
}

vec2 Text::getSize() {
    return MeasureTextEx(sys::font, text.c_str(), (float)size, 0);
}

void Text::draw() {
    Node::draw();
    DrawTextEx(sys::font, text.c_str(), pos, (float)size, 0, color);
}
