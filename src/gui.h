#pragma once

#include <string>

#include <raylib.h>

#include "maths.h"
#include "node.h"
#include "gfx/text.h"

struct Button : public Node {
    using Node::Node;

    typedef void (*CallBack)(void *udata, int id);

    enum State {
        Normal, Hover, Pressed, Count
    };

    Button() = default;
    Button(const char *new_text, vec2i pos, vec2i size, int new_id, CallBack cb = nullptr, void *udata = nullptr);

    virtual void init() override;
    virtual void cleanup() override;
    virtual void update() override;
    virtual void draw() override;

    recti dst;
    Texture textures[Count];
    State state = Normal;
    NPatchInfo info;
    int id = 0;
    void *userdata = nullptr;
    CallBack callback = nullptr;
    bool was_down = false;
    bool was_inside_when_pressed = false;

    Text text;
};