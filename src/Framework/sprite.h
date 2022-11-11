#pragma once

#include <raylib.h>
#include "../maths.h"

struct Sprite {
    inline void draw() {
        if (tex_rect.w == 0.f && tex_rect.h == 0.f) {
            tex_rect.size = { (float)tex.width, (float)tex.height };
        }
        vec2 size = { (float)tex.width, (float)tex.height };
        rect dst = { pos, size };
        DrawTexturePro(tex, tex_rect, dst, origin, angle, WHITE);
    }

    Texture tex = {};
    rect tex_rect;
    vec2 pos = {};
    vec2 origin = {};
    float angle = 0.f;
    Color color = WHITE;
};