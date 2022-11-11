#pragma once

#include <raylib.h>
#include "maths.h"

struct View {
    vec2 offset = { 0, 0 };
    vec2 target = { 0, 0 };
    float rotation = 0.f;
    float zoom = 1.f;

    operator Camera2D() const { return { offset, target, rotation, zoom }; }
};

extern View g_view;