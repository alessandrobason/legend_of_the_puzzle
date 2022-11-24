#pragma once

#include <string>

#include <raylib.h>

#include "maths.h"
#include "node.h"
#include "gfx/text.h"
#include "gfx/sprite.h"


struct Button : public Node {
    using Node::Node;

    typedef void (*CallBack)(void *udata, int id);

    enum State {
        Normal, Hover, Pressed, Count
    };

    Button() = default;
    Button(const char *new_text, vec2i pos, vec2i size, int new_id, CallBack cb = nullptr, void *udata = nullptr);

    virtual void init() override;
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

struct CheckBox : public Node {
    using Node::Node;

    typedef void (*CallBack)(void *udata, int id, bool is_down);

    enum State {
        Normal, Hover, Pressed, Count
    };

    CheckBox() = default;
    CheckBox(vec2i pos, bool default, int new_id = 0, CallBack cb = nullptr, void *udata = nullptr);

    virtual void update() override;
    virtual void draw() override;

    Atlas atlas;

    int id = 0;
    void *userdata = nullptr;
    CallBack callback = nullptr;

    State state = Normal;
    bool is_pressed = false;
    bool was_down = false;
    bool was_inside_when_pressed = false;
};

struct Slider : public Node {
    using Node::Node;

    typedef void (*CallBack)(void *udata, int id, float value);

    enum State {
        Normal, Hover, Pressed, Count
    };

    Slider() = default;
    Slider(vec2i pos, int width, float initial_value, int new_id = 0, CallBack cb = nullptr, void *udata = nullptr);

    virtual void update() override;
    virtual void draw() override;

    Atlas atlas;

    int id = 0;
    void *userdata = nullptr;
    CallBack callback = nullptr;

    State state = Normal;
    vec2i pos;
    int width = 0;
    float value = 0.f;
    // bool is_enabled = false;
    bool was_down = false;
    bool was_inside_when_pressed = false;
};